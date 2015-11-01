// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < -1) 
		throw out_of_range("out of range"); //чтобы длина поля не принимала отрицательных значений
	MemLen = (len+sizeof(TELEM)*8-1)/(sizeof(TELEM)*8); //16
	pMem = new TELEM[MemLen]; 
	if ( pMem != NULL ) 
		for ( int i=0; i < MemLen; i++ ) 
			pMem[i] = 0; 
	BitLen = len; 
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen=bf.BitLen; 
	MemLen=bf.MemLen; 
	pMem =new TELEM[MemLen]; 
	for (int i=0;i<MemLen; i++) 
		pMem[i]=bf.pMem[i];
}

TBitField::~TBitField()
{
	delete []pMem; 
	pMem = NULL;
}
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n/(sizeof(TELEM)*8); 
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n % sizeof(TELEM)*8);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
  
}

void TBitField::SetBit(const int n) // установить бит
{
	int a = GetMemIndex(n); 
	if ((n < 0) || (n > BitLen)) 
		throw 
		out_of_range("Out of range"); 
	pMem[GetMemIndex(n)] |= GetMemMask(n); 
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ( (n <0) || (n > BitLen) ) 
		throw out_of_range("Out of range"); 
	pMem[GetMemIndex(n)] &=~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  int a = GetMemIndex(n); 
	if ((n < 0) || (n > BitLen)) 
		throw 
		out_of_range("out of range"); 
	return pMem[a] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen; 
	MemLen = bf.MemLen; 
	delete []pMem; 
	pMem = new TELEM [MemLen]; 
	for (int i=0;i<MemLen;i++) 
		pMem[i] = bf.pMem[i]; 
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
int flag; 
	if (MemLen == bf.MemLen) 
	{ 
		for (int i=0;i<MemLen;i++) 
			if (pMem[i] != bf.pMem[i]) 
			{ 
				flag = 0; 
				break; 
			} 
			flag = 1; 
	} 
	return flag; 
  
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
   int flag; 
	if (MemLen == bf.MemLen) 
	{ 
		for (int i=0;i<MemLen;i++) 
			if (pMem[i] = bf.pMem[i]) 
			{ 
				flag = 0; 
				break; 
			} 
			flag = 1; 
	} 
	return flag; 
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{	int len ;
	if (BitLen >= bf.BitLen) 
		len = BitLen; 
	else len = bf.BitLen; 
	TBitField x(len); //new object
	for (int i=0;i<x.MemLen;i++) 
	{ 
		x.pMem[i] = bf.pMem[i] | pMem[i]; 
	} 
	return x; 
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{ 
	int len; 
	if (BitLen >= bf.BitLen) len = BitLen; 
	else len = bf.BitLen; 
	TBitField x(len); 
	for (int i=0;i<x.MemLen;i++) 
	{ 
		x.pMem[i] = bf.pMem[i] & pMem[i]; 
	} 
	return x; 
}

TBitField TBitField::operator~(void) // отрицание
{
	int len=BitLen;
	TBitField bf2(len);
	for(int i=0; i < (MemLen-1); i++)
		bf2.pMem[i] = ~pMem[i];

	for (int i = (MemLen-1)*sizeof(TELEM); i < BitLen; i++)
	{
		if ((*this).GetBit(i) == 0)
			bf2.SetBit(i);
	}
	return bf2;

	
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i=0; char ch; 
	do { istr >> ch; } while (ch !=' '); 
	while (1) 
	{ 
		istr >> ch; 
		if ( ch == '0' ) bf.ClrBit(i++); //очищаем бит
		else if ( ch == '1' ) bf.SetBit(i++); //уст бит
		else break; 
	} 
	return istr; 
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int len=bf.GetLength();
	for (int i=0;i<len;i++)
		if (bf.GetBit(i)) ostr <<'1'; else ostr<<'0';
	
	return ostr;
}
