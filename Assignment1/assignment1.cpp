//#include "common.h"

#ifndef _common_h_
#include "common.h"
#define _common_h_
#endif

//////////////////////////////////////////////////////////////
//
//			Some common functions
//
//////////////////////////////////////////////////////////////
digitList *digitize(char str[80])
{
	digitList*	L = 0;
	digitList*	node;

	int i;
	
	for(i = 0; i< strlen(str); i++)
	{
		/*if(str[i] < '0' || str[i] > '9')	break;

		node = new digitList(str[i] - '0', L);
		L = node;*/

		//-------------------------------------------//
		if ((str[i] >= '0') && (str[i] <= '9'))
		{
			node = new digitList(str[i] - '0', L);
			L = node;
		}
		else if (str[i] == '!')
		{
			if (((L->getDigit() == 0) && (L->getNextDigit() == NULL)) || ((L->getDigit() == 1) && (L->getNextDigit() == NULL)))	return new digitList(1, NULL);

			Integer P(1);
			int j = 1, cmp = -1;

			while (cmp == -1)
			{
				P = P * Integer(j);
				cmp = compareDigitLists(digitize(j), L);
				++j;
			}

			return P.digits;
		}
		else if (str[i] == '^')
		{
			while (*str != '^')
				str = str + 1;
			digitList* exp;
			exp = digitize(str + 1);

			if ((exp->getDigit() == 0) && (exp->getNextDigit() == NULL))	return new digitList(1, NULL);

			Integer P(1);
			Integer base(L);
			int count = 0, cmp = -1;

			while (cmp == -1)
			{
				P = P * base;
				++count;
				cmp = compareDigitLists(digitize(count), exp);
			}

			return P.digits;
		}
		else
		{
			break;
		}
	}

	return L;

}

digitList *trimDigitList(digitList* L)
{
	// dung phuong thuc rightDigits thuoc lop digiList de lay 1 phan tu tan cung
	// ben phai, kiem tra xem phan tu do co bang khong (0) hay khong.
	// neu no bang khong, chay tiep rightDigits voi 2 phan tu.
	// ...
	// chay den khi het so 0 tan cung be phai.
	// trong qua trinh chay dung mot bien n de dem, n = (so so 0) + 1
	// di chuyen con tro pTemp den vi tri con so khac 0 cuoi cung cua chuoi L, gan con tro nextDigit = NULL
	
	if (L == NULL) return L;

	int n = 1;
	digitList* List;
	List = L->rightDigits(n);

	if (List == NULL)			return L;
	else if (List->digit != 0)	return L;
	else 
	{
		n++;
		digitList* pTemp = L;
		digitList* zeroList;

		while (1)
		{
			zeroList = L->rightDigits(n);
			if (zeroList == NULL)
				break;
			else
			{
				if (zeroList->digit == 0)
					n++;
				else
					break;
			}
		}

		for(int i = 0; i < L->length() - n; i++)
			pTemp = pTemp->getNextDigit();

		pTemp->nextDigit = NULL;

		return L;
	}
}

digitList *subDigitLists(int b, digitList* L1, digitList* L2)
{
	if ((L1 == NULL) && (L2 == NULL))
		return NULL;
	else if (L1 == NULL)
		return subDigitLists(b, L2, NULL); //this case never happens because L1 is always longer than or equal to L2
	else if (L2 == NULL)
	{
		int t = L1->getDigit() - b;
		if (t < 0)
		{
			t = t + radix;
			return new digitList(t, subDigitLists(1, L1->getNextDigit(), NULL));
		}
		else
		{
			return new digitList(t, subDigitLists(0, L1->getNextDigit(), NULL));
		}
	}
	else
	{
		int t = L1->getDigit() - L2->getDigit() - b;
		if (t < 0) 
		{
			t = t + radix;
			return new digitList(t, subDigitLists(1, L1->getNextDigit(), L2->getNextDigit()));
		}
		else
		{
			return new digitList(t, subDigitLists(0, L1->getNextDigit(), L2->getNextDigit()));
		}
	}
}

Integer computeValue(int operatorNum)
{
	Integer		L1, L2;

	L1 = operandArr[0];

	for(int i = 0; i<operatorNum; i++)
	{
		L2 = operandArr[i+1];
		switch(operatorArr[i])
		{
		case '+':
			L1 = L1 + L2;
			break;
		case '-':
			L1 = L1 - L2;
			break;
		case '*':
			L1 = L1 * L2;
			break;
		}
	}
	
	return L1;
}
///////////////////////////////////////////////////////////////////////////
//
//		Some methods of Integer class
//
////////////////////////////////////////////////////////////////////
Integer Integer::operator +(Integer L)
{
	if(sign == L.sign)
		return Integer(sign, addDigitLists(0, digits, L.digits));
	else
	{
		int i = compareDigitLists(digits, L.digits);
		if (sign == 1)
		{
			if ((i == 1) || (i == 0))
				return Integer(1, subDigitLists(0, digits, L.digits)).trimDigit();
			else
				return Integer(-1, subDigitLists(0, L.digits, digits));
		}
		else
		{
			if ((i == -1) || (i == 0))
				return Integer(1, subDigitLists(0, L.digits, digits)).trimDigit();
			else
				return Integer(-1, subDigitLists(0, digits, L.digits));
		}
	}
}

Integer Integer::operator -(Integer L)
{
	int i = compareDigitLists(digits, L.digits);
	if (sign == L.sign)
	{
		if (sign == 1)
		{
			if ((i == 1) || (i == 0))
				return Integer(1, subDigitLists(0, digits, L.digits)).trimDigit();
			else
				return Integer(-1, subDigitLists(0, L.digits, digits)).trimDigit();
		}
		else
		{
			if ((i == -1) || (i == 0))
				return Integer(1, subDigitLists(0, L.digits, digits)).trimDigit();
			else
				return Integer(-1, subDigitLists(0, digits, L.digits)).trimDigit();
		}
	}
	else
	{
		return Integer(sign, addDigitLists(0, digits, L.digits));
	}
}

Integer Integer::leftDigits(int n)
{
	return Integer(digits->leftDigits(n));
}

Integer Integer::rightDigits(int n)
{
	return Integer(digits->rightDigits(n));
}

Integer	Integer::shift(int n)
{
	for (int i = 0; i < n; i++)
	{
		digitList* pTemp = new digitList();
		pTemp->nextDigit = digits;
		digits = pTemp;
	}
	return Integer(digits);
}

Integer Integer::operator *(Integer Y)
{
	digitList* pTail = Y.digits;

	while (length() > Y.length())
	{
		while (pTail->getNextDigit() != NULL)
			pTail = pTail->getNextDigit();
		digitList* pTemp = new digitList();
		pTail->nextDigit = pTemp;
		pTail = pTemp;
	}

	if (length() < Y.length())
	{
		Integer* pThis = this;
		return Y * (*pThis);
	}

	Integer P;
	if (digits->getNextDigit() == NULL)
		P = Integer(digits->digit * Y.digits->digit);
	else
	{
		Integer P1, P2, P3, P4;
		int l = length();
		int n = l / 2;

		P1 = leftDigits(n) * Y.leftDigits(n);

		P2 = rightDigits(l - n) * Y.leftDigits(n);

		P3 = leftDigits(n) * Y.rightDigits(l - n);

		P4 = rightDigits(l - n) * Y.rightDigits(l - n);

		P2 = P2 + P3;
		P2 = P2.shift(n);
		P4 = P4.shift(2 * n);

		P = P1 + P2;
		P = P + P4;
	}

	// truong hop nhan mot so voi so khong
	if (P.digits == NULL)
	{
		digitList* node = new digitList();
		P.digits = node;
	}

	P = P.trimDigit();

	if ((sign == Y.sign) || ((P.digits->getDigit() == 0) && (P.digits->getNextDigit() == NULL)))	return Integer(1, P.digits);
	else																							return Integer(-1, P.digits);
}
