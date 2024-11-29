// g++ -g -Wall CVector.cpp -o cvector
#include <iostream>
#define MAX_SIZE 10 // 벡터의 최대 크기

class CVector {
private:
	int mSize; // 벡터의 차원
public: // public, private(기본값), protected
	double mV[MAX_SIZE]; // 데이터(배열)

	CVector() {}; //기본 생성자(constructor)
	~CVector() {}; //기본 소멸자(destructor)

	void SetSize(int n) { mSize = n; };
	int GetSize() { return mSize; };
	void Print(const char* msg = nullptr);
	void ScalarMul(double alpha); // 자기자신을 상수배
	CVector Add(CVector x); // 자신(벡터) + x(벡터) ==> 리턴(벡터)

	// HW2 - 1
	static CVector Add(CVector x_vec, CVector y_vec);
    CVector operator+(CVector& vec);
	// CVector& operator=(const CVector& vec);
};

// 벡터 덧셈 멤버 함수
CVector CVector::Add(CVector x) {
	//유효한지 확인
	if (mSize != x.mSize) {
		printf("Vector size error! \n");
		exit(1);
		// return x; // 의미없는 리턴값 
	}
	CVector result_vec;
	result_vec.mSize = mSize;
	for (int i = 0; i < mSize; i++) {
		result_vec.mV[i] = mV[i] + x.mV[i];
	}
	return result_vec;
}

// 상수배 멤버 함수 (업데이트 방식)
void CVector::ScalarMul(double alpha) {
	for (int i = 0; i < mSize; i++) mV[i] *= alpha;
}

//벡터 출력 멤버 함수
void CVector::Print(const char* msg) {
	if (msg != nullptr) {
		printf("%s = ", msg);
	}
	printf("[ ");
	for (int i = 0; i < mSize; i++) {
		printf(" %7.3f", mV[i]);
	}
	printf(" ]\n");
}

void Vector_test() {
	CVector v1, v2, v3;

	v1.SetSize(3);
	v2.SetSize(3);
	v3.SetSize(3);
	for (int i = 0; i < v1.GetSize(); i++) {
		v1.mV[i] = i + 1.0;
		v2.mV[i] = 1.0;
	}

	v1.ScalarMul(5.0);
	v3 = v2.Add(v1);

	v1.Print("v1");
	v2.Print("v2");
	v3.Print("v3");

}

// HW2
CVector CVector::Add(CVector x_vec, CVector y_vec) {
	if (x_vec.mSize != y_vec.mSize) {
		fprintf(stderr, "Vector size error!\n");
		exit(1);
	}
	
	CVector z_vec; 	// z = x + y
	z_vec.mSize = x_vec.mSize;
	
	for (int i = 0; i < z_vec.mSize; i++)
		z_vec.mV[i] = x_vec.mV[i] + y_vec.mV[i];

	return z_vec;
}


CVector CVector::operator+(CVector& vec) {
	CVector new_vec;
	double new_elemt;

	if (mSize != vec.mSize) {
		fprintf(stderr, "Vector size error!\n");
		exit(1);
	}

	new_vec.SetSize(vec.mSize);

	for (int i = 0; i < new_vec.mSize; i++) {
		new_elemt = mV[i] + vec.mV[i];
		new_vec.mV[i] = new_elemt;
	}

	return new_vec;
}
// CVector& CVector::operator=(const CVector& vec) {
// 	if (this != &vec) {
// 		mSize = vec.mSize;

// 		for (int i = 0; i < mSize; i++)
// 			mV[i] = vec.mV[i];
// 	}
// 	return *this;
// }

void Vector_test2() {
	CVector v1, v2, v3, v4, v5;
	v1.SetSize(3);
	v2.SetSize(3);
	v3.SetSize(3);
	for (int i = 0; i < v1.GetSize(); i++) {
		v1.mV[i] = i + 1.0;
		v2.mV[i] = 1.0;
	}
	v1.ScalarMul(5.0);
	v3 = v2.Add(v1);
	v4 = CVector::Add(v1, v2);
	v5 = v1 + v2;

	v1.Print("v1");
	v2.Print("v2");
	v3.Print("v3");
	v4.Print("v4");
	v5.Print("v5");
}

int main()
{
	// Vector_test();
	Vector_test2();
}