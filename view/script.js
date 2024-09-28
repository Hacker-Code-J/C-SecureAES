function updateUI(matrix, stepName) {
    const matrixContainer = document.getElementById('matrixContainer');
    const stepDescription = document.getElementById('stepDescription');

    // Clear existing content
    matrixContainer.innerHTML = '';
    stepDescription.innerHTML = '';

    // Update the matrix display
    matrix.forEach(row => {
        row.forEach(cell => {
            const cellDiv = document.createElement('div');
            cellDiv.textContent = cell.toString(16).toUpperCase(); // Convert to hex and display
            matrixContainer.appendChild(cellDiv);
        });
    });

    // Update the step description
    stepDescription.textContent = 'Current Step: ' + stepName;
}

function initializeMatrix(plaintext) {
    // Convert plaintext to a 4x4 matrix (16 bytes)
    let matrix = new Array(4);
    for (let i = 0; i < 4; i++) {
        matrix[i] = new Array(4);
        for (let j = 0; j < 4; j++) {
            matrix[i][j] = plaintext[i * 4 + j];
        }
    }
    updateUI(matrix, "Initialization");
}


function startEncryption() {
    // Example plaintext and key (in reality, you would get these from user input)
    let plaintext = "exampleplaintext"; // This should be 16 characters (16 bytes)
    let key = "examplekeyexample"; // Depending on AES variant, this could be 16, 24, or 32 characters

    // Convert plaintext and key into a format suitable for AES (e.g., 4x4 matrix)
    let matrix = convertToMatrix(plaintext);
    let expandedKey = generateRoundKeys(key);

    // Initial UI update
    updateUI(matrix, "Initial Matrix");

    // The number of rounds depends on the key size: 10 for 128-bit keys, 12 for 192-bit keys, 14 for 256-bit keys
    let numberOfRounds = 10; // Assuming AES-128 for simplicity

    // Initial round key addition
    addRoundKey(matrix, expandedKey[0]);
    updateUI(matrix, "AddRoundKey (Initial)");

    // Main rounds
    for (let round = 1; round <= numberOfRounds; round++) {
        subBytes(matrix);
        updateUI(matrix, "SubBytes");
        
        shiftRows(matrix);
        updateUI(matrix, "ShiftRows");
        
        if (round < numberOfRounds) { // MixColumns is not performed in the final round
            mixColumns(matrix);
            updateUI(matrix, "MixColumns");
        }

        addRoundKey(matrix, expandedKey[round]);
        updateUI(matrix, "AddRoundKey");
    }

    // Final state is the encrypted data
    updateUI(matrix, "Encryption Complete");
}


function subBytes(matrix) {
    // Replace each byte with its corresponding value in the S-box
    for (let i = 0; i < 4; i++) {
        for (let j = 0; j < 4; j++) {
            matrix[i][j] = sBox[matrix[i][j]];
        }
    }
    updateUI(matrix, "SubBytes");
}


function shiftRows(matrix) {
    // Shift rows left by their row number
    for (let i = 1; i < 4; i++) {
        matrix[i] = shiftArrayLeft(matrix[i], i);
    }
    updateUI(matrix, "ShiftRows");
}


function mixColumns(matrix) {
    // Mix columns using Galois field arithmetic
    // This involves matrix multiplication with a fixed matrix
    // Pseudocode here
    updateUI(matrix, "MixColumns");
}


function addRoundKey(matrix, roundKey) {
    // XOR each byte with the corresponding byte in the round key
    for (let i = 0; i < 4; i++) {
        for (let j = 0; j < 4; j++) {
            matrix[i][j] ^= roundKey[i][j];
        }
    }
    updateUI(matrix, "AddRoundKey");
}

function shiftArrayLeft(array, times) {
    const length = array.length;
    times = times % length; // Ensure times is within the array bounds
    return array.slice(times).concat(array.slice(0, times));
}


function encryptAES(plaintext, key) {
    let matrix = initializeMatrix(plaintext);
    let roundKeys = generateRoundKeys(key);

    // Perform the required number of rounds
    for (let round = 0; round < numberOfRounds; round++) {
        subBytes(matrix);
        shiftRows(matrix);
        if (round < numberOfRounds - 1) { // Skip MixColumns in the last round
            mixColumns(matrix);
        }
        addRoundKey(matrix, roundKeys[round]);
    }

    return matrix;
}

// Initialize the matrix when the page loads
window.onload = initializeMatrix;

// Add more functions as necessary
