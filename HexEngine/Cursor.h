#pragma once

class Cursor {
public:
	// singleton stuff
	static Cursor* GetInstance();
	static void Destroy();

private:
	// singleton stuff
	static Cursor *instance;

	Cursor();
	~Cursor();
	Cursor(Cursor const&);
	void operator=(Cursor const&);
};