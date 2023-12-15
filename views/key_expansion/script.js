// Assuming you have the key as an array of bytes:
let key = new Array(16).fill(0);  // Replace with actual key bytes

// Define the Rcon array (round constant)
const Rcon = [
  [0x01, 0x00, 0x00, 0x00], /* ... other constants ... */
];

// SubWord function (substitutes bytes using the S-box)
function SubWord(word) {
  // Implement the SubWord functionality using the AES S-box
}

// RotWord function (circular byte left shift)
function RotWord(word) {
  return [word[1], word[2], word[3], word[0]];
}

// Key expansion code
function keyExpansion(key) {
  let words = [];
  let i = 0;

  // The first 4 words are the original key
  for (i = 0; i < 4; i++) {
    words[i] = key.slice(i * 4, (i + 1) * 4);
  }

  for (i = 4; i < 44; i++) {
    let temp = words[i - 1];
    if (i % 4 === 0) {
      temp = SubWord(RotWord(temp));
      temp[0] = temp[0] ^ Rcon[i/4][0];
    }
    words[i] = [];
    for (let j = 0; j < 4; j++) {
      words[i][j] = words[i - 4][j] ^ temp[j];
    }
  }
  return words;
}

let expandedKey = keyExpansion(key);

// Create the table with the expanded key data
function createTable(expandedKey) {
  let tableHTML = "<tr><th>Word</th><th>Bytes</th></tr>";
  for (let i = 0; i < expandedKey.length; i++) {
    tableHTML += "<tr>";
    tableHTML += `<td>Word ${i}</td>`;
    tableHTML += `<td>${expandedKey[i].map(byte => byte.toString(16).padStart(2, '0')).join(' ')}</td>`;
    tableHTML += "</tr>";
  }
  return tableHTML;
}

document.getElementById('key-expansion-table').innerHTML = createTable(expandedKey);