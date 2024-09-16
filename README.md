# CSCI-4530 Computer Security Project 1:

## Cracking a Substitution Cipher

The goal of this project is to decrypt the given ciphertext using a substitution cipher. To assist with this, the given input text is already shifted to fit the most common letter frequencies in the English dictionary.

This program follows a simple structure:

1. The score of your current mapping is displayed.
2. The current mapping of the letters is displayed.
3. The state of the current ciphertext is displayed.

You will then have the option to swap letters. When prompted, choose two letters to swap. They will swap, and you can repeat the process until you have fully deciphered the text.

Once you think you have solved the cipher, exit the loop and navigate to `build/ProjectDoc/deciphertext.txt` to check if the key you have produced is the same as the true key.

### How to run the program

1. Navigate to the build folder <code>cd build</code>
2. Run the command <code>CMake .. </code>
3. Run <code>make</code>
4. Run <code>./Project1</code>
5. Follow the prompts given in the termial and solve the cipher!