import os
import sys
def int2bytes(num:int, n_bytes:int = 4, little_endian=True):
    gen_bytes = bytearray(n_bytes)
    for i in range(n_bytes):
        gen_bytes[i] = (num >> (i * 8)) & 0xff
    if not little_endian:
        # big edian output
        gen_bytes.reverse()
    return gen_bytes

def str2bytes(s:str):
    b = bytearray(s.encode('utf8'))
    b.append(0)
    return b

N_BYTES = 4
if __name__ == "__main__": # arg1: path_folder, arg2: path_image, 
    little_endian = True # Choose between: little-endian or big-endian
    if len(sys.argv) < 3:
        print("Please Speciy input director and output filename")
        exit()    
    path_folder = sys.argv[1]
    path_image = sys.argv[2]
    image = bytearray()
    filenodes = []
    print(f"Entering folder {path_folder}...")
    try:
        os.chdir(path_folder)
    except:
        print(f'{path_folder} does not exist.')
        exit()
    list_files = list(os.listdir())
    n_files = len(list_files)
    # 0: base_addr: Number of files
    image.extend(int2bytes(n_files, N_BYTES))
    print(len(list_files), 'files found')
    for fname in list_files:
        print(f'Processing {fname}...')
        with open(fname, 'rb') as f:
            content_f = f.read()
            size_f = len(content_f)
            name_f = '/'+fname
            filenodes.append({'size': size_f, 'content': content_f, 'name': name_f})
    print('Done. Start generating image file')
    node_addr = N_BYTES * (n_files + 1)
    image_node = bytearray()
    for node in filenodes:
        image.extend(int2bytes(node_addr, N_BYTES))
        image_node.extend(int2bytes(node['size'], N_BYTES)) # node: size
        name_bytes = str2bytes(node['name'])
        content_addr = node_addr + 2*N_BYTES + len(name_bytes)
        image_node.extend(int2bytes(content_addr, N_BYTES))  # node: content_addr
        image_node.extend(name_bytes) # node: filename\0
        image_node.extend(node['content'])
        ### !!!! 4-byte alignment
        len_stuffing = (-len(image_node)) % N_BYTES
        image_node.extend([0] * len_stuffing) # byte stuffing for alignment
        print('____________________')
        print('size:', image_node[:4].hex())
        print('content_addr', int2bytes(content_addr, N_BYTES).hex())
        print('name:', str2bytes(node['name']).hex())
        node_addr = content_addr + node['size'] + len_stuffing
    image.extend(image_node)
    os.chdir('..')
    with open(path_image, 'wb') as f:
        f.write(image)
    with open(path_image + '.txt', 'w') as f:
        i = 0
        for b in image:
            i+=1
            f.write(hex(b))
            f.write(', ')
            if(i == 16):
                i = 0
                f.write('\n')
    print('All Done')
    print('Space occupancy:', len(image))
    print(f'Memory Span: {hex(len(image))}')