#pragma once
class TObject1
{
private:
	int size;
public:
	TObject1(int size) {
		this->size = size;
	}

	int __GET__(int index) { return 0; }
	void __SET__(int index, int value) {}
	virtual const char* toString() { return 0; }
	~TObject1();
};

