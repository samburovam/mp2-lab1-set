// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0) throw "Error";
	BitLen = len;
	MemLen = (BitLen - 1) / (sizeof(TELEM) * 8) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return (1 << (n % (sizeof(TELEM) * 8)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n >= BitLen)) throw "Error";
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n >= BitLen)) throw "Error";
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n >= BitLen)) throw "Error";
	return (pMem[GetMemIndex(n)] & GetMemMask(n)) >> (n % (sizeof(TELEM) * 8));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;
	for (int i = 0; i < BitLen; i++)
		if (GetBit(i) != bf.GetBit(i)) return 0;
	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 1;
	for (int i = 0; i < BitLen; i++)
		if (GetBit(i) != bf.GetBit(i)) return 1;
	return 0;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	TBitField res(0);
	if (BitLen > bf.BitLen)
		res.BitLen = BitLen;
	else
		res.BitLen = bf.BitLen;
	for (int i = 0; i < MemLen; i++)
		res.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		res.pMem[i] = res.pMem[i] | bf.pMem[i];
	return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	int sch = 0;
	if (BitLen >= bf.BitLen)
		sch = 1;
	TBitField res(0);
	if (sch == 1)
		res.BitLen = BitLen;
	else
		res.BitLen = bf.BitLen;
	if (sch == 1)
	{
		for (int i = 0; i < bf.MemLen; i++)
			res.pMem[i] = bf.pMem[i];
		for (int i = 0; i < MemLen; i++)
			res.pMem[i] = res.pMem[i] & pMem[i];
	}
	else
	{
		for (int i = 0; i < MemLen; i++)
			res.pMem[i] = pMem[i];
		for (int i = 0; i < bf.MemLen; i++)
			res.pMem[i] = res.pMem[i] & bf.pMem[i];
	}
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i = 0; i < BitLen; i++)
	{
		if (GetBit(i) == 0)
			res.SetBit(i);
	}
	return res;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	int get;
	for (int i = 0; i < bf.BitLen; i++) {
		istr >> get;
		bf.SetBit(get);
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
		ostr << (bf.GetBit(i));
	return ostr;
}
