#pragma once
#include "framework.h"

struct PacketData {
	WORD Size;
	BYTE Data[];

	DWORD RawSize() CONST {
		return DWORD(Size) + 2;
	}
	
	VOID Copy(CONST LPBYTE Src) {
		CopyMemory(Data, Src, Size);
	}
	VOID Wipe() {
		SecureZeroMemory(Data, Size);
	}
};

class PacketBase {
protected:
	PacketData* P;

	PacketBase(PacketBase& Src) : P(Src.P) { Src.P = NULL; }

public:
	PacketBase() : P(NULL) {}

	VOID Set(CONST LPBYTE New) {
		P = (PacketData*)New;
	}
	VOID Alloc(WORD Bytes) {
		Set(new(nothrow) BYTE[DWORD(Bytes) + 2]);
		if (P) {
			P->Size = Bytes;
		}
	}
	VOID Discard() {
		delete[] LPBYTE(P);
		P = NULL;
	}
	VOID Copy(CONST PacketBase& Src) {
		Alloc(Src.P->Size);
		if (P) {
			P->Copy(Src.P->Data);
		}
	}

	PacketData* operator->() CONST { return P; }
	PacketData* operator&() CONST { return P; }
	BOOL operator!() CONST { return !P; }
};