import heapq
import os
import argparse
import struct
from collections import Counter, defaultdict

class Node:
    def __init__(self, byte, freq):
        self.byte = byte
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq

def build_huffman_tree(data):
    freq = Counter(data)
    heap = [Node(byte, freq[byte]) for byte in freq]
    heapq.heapify(heap)

    while len(heap) > 1:
        node1 = heapq.heappop(heap)
        node2 = heapq.heappop(heap)
        merged = Node(None, node1.freq + node2.freq)
        merged.left = node1
        merged.right = node2
        heapq.heappush(heap, merged)

    return heap[0]

def build_codes(node, prefix="", codebook=None):
    if codebook is None:
        codebook = {}
    if node:
        if node.byte is not None:
            codebook[node.byte] = prefix
        build_codes(node.left, prefix + "0", codebook)
        build_codes(node.right, prefix + "1", codebook)
    return codebook

def encode_data(data, codebook):
    encoded_bits = ''.join(codebook[byte] for byte in data)
    padding = 8 - len(encoded_bits) % 8
    encoded_bits += '0' * padding
    byte_array = bytearray()

    for i in range(0, len(encoded_bits), 8):
        byte_array.append(int(encoded_bits[i:i+8], 2))

    return bytes(byte_array), padding

def decode_data(encoded_data, tree, padding):
    bits = ''.join(f'{byte:08b}' for byte in encoded_data)
    bits = bits[:-padding]
    result = bytearray()

    node = tree
    for bit in bits:
        node = node.left if bit == '0' else node.right
        if node.byte is not None:
            result.append(node.byte)
            node = tree

    return bytes(result)

def write_tree(node, fout):
    if node.byte is not None:
        fout.write(b'1')
        fout.write(struct.pack('B', node.byte))
    else:
        fout.write(b'0')
        write_tree(node.left, fout)
        write_tree(node.right, fout)

def read_tree(fin):
    flag = fin.read(1)
    if flag == b'1':
        byte = struct.unpack('B', fin.read(1))[0]
        node = Node(byte, 0)
        return node
    else:
        node = Node(None, 0)
        node.left = read_tree(fin)
        node.right = read_tree(fin)
        return node

def compress(files, archive_name):
    all_data = bytearray()
    file_infos = []

    for file in files:
        with open(file, 'rb') as f:
            data = f.read()
            all_data.extend(data)

    tree = build_huffman_tree(all_data)
    codebook = build_codes(tree)

    with open(archive_name, 'wb') as fout:
        fout.write(struct.pack('I', len(files)))

        for file in files:
            with open(file, 'rb') as f:
                data = f.read()
                encoded, padding = encode_data(data, codebook)
                file_name_bytes = file.encode()
                fout.write(struct.pack('H', len(file_name_bytes)))
                fout.write(file_name_bytes)
                fout.write(struct.pack('I', len(encoded)))
                fout.write(struct.pack('B', padding))
                fout.write(encoded)

        write_tree(tree, fout)

def decompress(archive_name, output_dir):
    with open(archive_name, 'rb') as fin:
        num_files = struct.unpack('I', fin.read(4))[0]
        file_infos = []

        for _ in range(num_files):
            name_len = struct.unpack('H', fin.read(2))[0]
            name = fin.read(name_len).decode()
            size = struct.unpack('I', fin.read(4))[0]
            padding = struct.unpack('B', fin.read(1))[0]
            data = fin.read(size)
            file_infos.append((name, data, padding))

        tree = read_tree(fin)

        for name, data, padding in file_infos:
            decoded = decode_data(data, tree, padding)
            out_path = os.path.join(output_dir, name)
            os.makedirs(os.path.dirname(out_path), exist_ok=True)
            with open(out_path, 'wb') as f:
                f.write(decoded)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Archivator on Huffman Coding')
    parser.add_argument('mode', choices=['compress', 'decompress'], help='Mode')
    parser.add_argument('--files', nargs='+', help='Files list to compress')
    parser.add_argument('--archive', help='Archive name')
    parser.add_argument('--outdir', help='Folder for decompress')

    args = parser.parse_args()

    if args.mode == 'compress':
        compress(args.files, args.archive)
        print(f"Compressed into {args.archive}")

    elif args.mode == 'decompress':
        decompress(args.archive, args.outdir)
        print(f"Decompressed into {args.outdir}")

