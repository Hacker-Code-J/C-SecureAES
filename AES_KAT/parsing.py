def parse_and_output(file_path):
    with open(file_path, 'r') as file:
        # Initialize variables
        mode = None  # To track current block: None, 'PLAINTEXT', or 'CIPHERTEXT'
        lines = file.readlines()
        plaintexts = []
        ciphertexts = []

        for line in lines:
            # Remove any leading/trailing whitespace
            stripped_line = line.strip()

            # Skip empty lines
            if not stripped_line:
                continue

            # Update the mode based on the block identifier
            if stripped_line in ['PLAINTEXT', 'CIPHERTEXT']:
                mode = stripped_line
                continue

            # Append line to the respective list based on the mode
            if mode == 'PLAINTEXT':
                plaintexts.append(stripped_line)
            elif mode == 'CIPHERTEXT':
                ciphertexts.append(stripped_line)

        # Print the formatted output
        for pt, ct in zip(plaintexts, ciphertexts):
            print('KEY = 00000000000000000000000000000000')
            print(f'PT = {pt}')
            print(f'CT = {ct}\n')

# Replace 'path_to_file.txt' with the actual path to your .txt file
parse_and_output('128(VARTXT)KAT.txt')