#include "HexByte.h"

bool isHex(char ch) {
	return ((ch >= '0') && (ch <= '9')) || 	// 48 ~ 57 
		   ((ch >= 'A') && (ch <= 'F')) || 	// 65 ~ 70
		   ((ch >= 'a') && (ch <= 'f')); 	// 97 ~ 102
}

// char to byte
// 'a' - > 10, '0' -> 0, 'f' -> 15
u8 Hex2Digit(char ch) {
	if (!isHex(ch)) {
		cout << ch << " is not a hex value." << endl;
		return -1;
	}
	if ((ch >= '0') && (ch <= '9')) {
		return  ch - '0'; 
	}
	else if ((ch >= 'A') && (ch <= 'F')) {
		return ch - 'A' + 10;
	}
	else if ((ch >= 'a') && (ch <= 'f')) {
		return ch - 'a' + 10;
	}
	cout << "Unknown error." << endl;
	return -1;
}

// h[] = { h[0], h[1] }
// { 'a', '1' } -> 1010 0001 = A1
u8 Hex2Byte(const char h[2]) {
	u8 upper, lower;
	upper = h[0];
	lower = h[1];
	if ((!isHex(upper)) || (!isHex(lower))) {
		cout << "Hex Error" << endl;
		return -1;
	}
	return Hex2Digit(upper) * 16 + Hex2Digit(lower);
}

// hex_str[] = "8d2e60..."
// hex_len = 32
// barr[] = { '8d', '2e', ... }
void Hex2Array(const char hex_str[], int hex_len, u8 barr[]) {
	char h[2];
	u8 b_value;
	for (int i = 0; i < hex_len / 2; i++) {
		h[0] = hex_str[2 * i];
		h[1] = hex_str[2 * i + 1];
		b_value = Hex2Byte(h);
		barr[i] = b_value;
	}
}

void print_b_array(u8 b_arr[], int len, const char* pStr) {
	if (pStr != nullptr) {
		printf("%s = ", pStr);
	}
	for (int i = 0; i < len; i++) {
		printf("%02x ", b_arr[i]);
	}
	printf("\n");
}

void copy_b_array(u8 src[], int len, u8 dest[]) {
	for (int i = 0; i < len; i++) {
		dest[i] = src[i];
	}
}

void xor_b_array(u8 data[], int len, u8 xor_arr[]) {
	for (int i = 0; i < len; i++) {
		data[i] ^= xor_arr[i];
	}
}

void hex_test() {
	cout << "isHex('a') = " << isHex('a') << endl;
	cout << "Hex2Digit('a') = " << (int)Hex2Digit('a') << endl;

	char H[2] = { 'f', 'f' };
	cout << "Hex2Byte(H) = " << (int)Hex2Byte(H) << endl;
	const char* HexString = "8d2e60365f17c7df1040d7501b4a7b5a";
	u8 HexArray[16];
	Hex2Array(HexString, 32, HexArray);
	cout << HexString << " = ";
	print_b_array(HexArray, 16);

	/*
	isHex('a') = 1
	Hex2Digit('a') = 10
	Hex2Byte(H) = 255
	8d2e60365f17c7df1040d7501b4a7b5a = 8d 2e 60 36 5f 17 c7 df 10 40 d7 50 1b 4a 7b 5a 
	*/
}