
#include <algorithm>
#include <vector>
#include "BigInteger.h"
#include <map>
#include "MiniElGamal.h"
#include <common/RegKey.h>

class ShankPair
{
public:
	unsigned int a;
	WONCryptMini::BigInteger b;

	ShankPair() { }
	ShankPair(unsigned int theA, const WONCryptMini::BigInteger &theB) : a(theA), b(theB) { }

	bool operator<(const ShankPair &theVal)
	{
		return b.cmp(theVal.b)<0;
	}
};

void WriteVector(std::vector<ShankPair> &theVector, int theNum)
{
	std::sort<std::vector<ShankPair>::iterator>(theVector.begin(),theVector.end());
	std::vector<ShankPair>::iterator anItr = theVector.begin();

	char aFileName[255];
	sprintf(aFileName,"d:\\sort%04d",theNum);
	FILE *aFile = fopen(aFileName,"wb");
	while(anItr!=theVector.end())
	{
		fwrite(&(anItr->a),4,1,aFile);
		WONCryptMini::RawBuffer aBuf = anItr->b.toByteArray();

		unsigned char aLen = aBuf.length();
		fwrite(&aLen,1,1,aFile);
		fwrite(aBuf.data(),1,aBuf.length(),aFile);
	
		anItr++;
	}

	fclose(aFile);
	theVector.clear();

}

// y = g^x (mod p)
// x = mj + i  m = sqrt(p), 0<=j<=m, 0<=i<=m
// y = (g^mj)(g^i) (mod p)
// y*(g^-i) = g^mj (mod p)

WONCryptMini::BigInteger shank(const WONCryptMini::BigInteger &y, const WONCryptMini::BigInteger &g, const WONCryptMini::BigInteger &p, const WONCryptMini::BigInteger &max)
{
	std::vector<ShankPair> ivec;
	std::vector<ShankPair> jvec;

	WONCryptMini::BigInteger m;
	
	if(max.cmp(p)<0)
		m = max.sqrt().add(1);	
	else
		m = p.sqrt().add(1);


	int nb;
	WONCryptMini::BigInteger recip = p.recip(nb);

	WONCryptMini::BigInteger gm = g.modExp(m,p);
	WONCryptMini::BigInteger gInv = g.inverseModN(p);


	std::cout << "p = " << p << std::endl;
	std::cout << "g = mod(" << g << ",p)" << std::endl;
	std::cout << "y = mod(" << y << ",p)" << std::endl;
	std::cout << "m = " << m << std::endl;
	std::cout << "gm = mod(" << gm << ",p)" << std::endl;
	std::cout << "gInv = mod(" << gInv << ",p)" << std::endl;

	WONCryptMini::BigInteger i(31700000);
	WONCryptMini::BigInteger AccumI(y.modMulRecip(gInv.modExp(i,p),p,recip,nb));
	WONCryptMini::BigInteger j(0), AccumJ(1);

	int aFileCount = 0;
/*
	while(i.cmp(m)<0)
	{
		ivec.push_back(ShankPair(i.GetInt64(),AccumI));
		AccumI = AccumI.modMulRecip(gInv,p,recip,nb);

		i.add(1);

		if(i.mod(100000).cmp(BigInteger::ZERO)==0)	
		{
			WriteVector(ivec,aFileCount);
			aFileCount++;

	//		exit(1);
		}
	}

	WriteVector(ivec,aFileCount);
	exit(1);*/
/*	
	while(j.cmp(m)<0)
	{
		jvec.push_back(ShankPair(j.GetInt64(),AccumJ));
		AccumJ = AccumJ.modMulRecip(gm,p,recip,nb);

		j.add(1);
		if(j.mod(100000).cmp(BigInteger::ZERO)==0)	
		{
			WriteVector(jvec,aFileCount);
			aFileCount++;
		}
	}

	WriteVector(jvec,aFileCount);
	exit(1);*/


	WONCryptMini::BigInteger x(1);
	return x;
}

struct PollardStruct
{
	WONCryptMini::BigInteger y;
	unsigned __int64 a,b,i;
};

WONCryptMini::BigInteger Pollard(const WONCryptMini::BigInteger &h, const WONCryptMini::BigInteger &g, const WONCryptMini::BigInteger &p, const WONCryptMini::BigInteger &order)
{
	WONCryptMini::Random aRandom;

	while(true)
	{
		PollardStruct aTable[8];


		WONCryptMini::BigInteger p3 = p.div(3);
		WONCryptMini::BigInteger p23 = p3.mul(2);

		int nb;
		WONCryptMini::BigInteger recip = p.recip(nb);

		WONCryptMini::BigInteger pMinusOne = p.sub(1);
		pMinusOne = order;
		unsigned __int64 pMinusOne64 = pMinusOne.GetInt64();

		WONCryptMini::BigInteger alpha = aRandom.next(32);
		alpha = alpha.mod(p);

		WONCryptMini::BigInteger y = g.modExp(alpha,p);

		unsigned __int64 a = alpha.GetInt64();
		unsigned __int64 b = 0;
		unsigned __int64 i = 0;

		for(int j=0; j<8; j++)
		{
			aTable[j].y = y;
			aTable[j].a = a;
			aTable[j].b = b;
			aTable[j].i = 0;
		}

		while(true)
		{
			if(y.cmp(p3)<0)
			{
				a++;
				y = g.modMulRecip(y,p,recip,nb);	
			}
			else if(y.cmp(p23)<0)
			{
				a*=2; b*=2;
				y = y.modMulRecip(y,p,recip,nb);
			}
			else
			{
				b++;
				y = h.modMulRecip(y,p,recip,nb);
			}
			a%=pMinusOne64;
			b%=pMinusOne64;


			i++;
			if(i%100000==0)
				printf("%d\n",i);

			bool startOver = false;
			for(int j=0; j<8; j++)
			{
				if(y.cmp(aTable[j].y)==0)
				{
					WONCryptMini::BigInteger x(a-aTable[j].a,a<aTable[j].a);
					WONCryptMini::BigInteger temp(aTable[j].b - b,aTable[j].b < b);

					x = x.mod(pMinusOne);
					temp = temp.mod(pMinusOne);

					if(temp.cmp(WONCryptMini::BigInteger::ZERO)==0)
					{
						startOver = true;
						break;
					}

					std::cout << "order = " << order << std::endl;
					std::cout << "temp = " << temp << std::endl;
					if(temp.gcd(pMinusOne).cmp(WONCryptMini::BigInteger::ONE)!=0)
					{
						startOver = true;
						break;
					}

					std::cout << "g = " << g << std::endl;
					std::cout << "p = " << p << std::endl;
					std::cout << "h = " << h << std::endl;
					std::cout << "alpha = " << alpha << std::endl;
					std::cout << "ai = " << WONCryptMini::BigInteger(a) << std::endl;
					std::cout << "aj = " << WONCryptMini::BigInteger(aTable[j].a) << std::endl;
					std::cout << "bi = " << WONCryptMini::BigInteger(b) << std::endl;
					std::cout << "bj = " << WONCryptMini::BigInteger(aTable[j].b) << std::endl;
					

					temp = temp.inverseModN(pMinusOne);

					std::cout << "1/(bj-bi) = " << temp << std::endl;
		
					x = x.mul(temp).mod(pMinusOne);

					std::cout << "Got it!   x = " << x << std::endl;
					return x;
				}	
			}

			if(startOver)
				break;

			if(i>=3*aTable[0].i)
			{
				for(int k=0; k<7; k++)
					aTable[k] = aTable[k+1];

				aTable[7].y = y;
				aTable[7].a = a;
				aTable[7].b = b;
				aTable[7].i = i;
			}
		}
	}

	return WONCryptMini::BigInteger::ZERO;

}

WONCryptMini::BigInteger Hellman(const WONCryptMini::BigInteger &y, const WONCryptMini::BigInteger &g, const WONCryptMini::BigInteger &p, const WONCryptMini::BigInteger &p1, int n1)
{
	WONCryptMini::BigInteger pMinusOne = p.sub(WONCryptMini::BigInteger::ONE);

	WONCryptMini::BigInteger p1Mult(1);

	WONCryptMini::BigInteger gFact = g.modExp(pMinusOne.div(p1.mul(p1Mult)),p);


	WONCryptMini::BigInteger x(0);
	
	for(int i=0; i<n1; i++)
	{
		WONCryptMini::BigInteger exponent = pMinusOne.div(p1.exp(i+1));  // (p-1)/(p1^(i+1))
		WONCryptMini::BigInteger gToTheMinusX = g.modExp(x.negate(),p);  // g^(-sum(ai*p1^i))
		WONCryptMini::BigInteger yFact = y.mul(gToTheMinusX); // y*g^(-sum(ai*p1^i))
		yFact = yFact.modExp(exponent,p);

		WONCryptMini::BigInteger gFact = g.modExp(exponent,p);


		WONCryptMini::BigInteger b;

		if(yFact.cmp(WONCryptMini::BigInteger::ONE)==0)
			b = 0;
		else
			b = Pollard(yFact,gFact,p,p1);
		
		b = b.mod(p1);

		x = x.add(b.mul(p1.exp(i)));
	}

	return x;
}

/*char VerifierStr[] = 
"\x30\x34\x02\x0d\x00\xed\x2d\xc4\xa6\x15\x9c\xed\x14\x29\x69\xf9\xa9\x02\x07\x03\x92\x9a\xf2\x2a\x60\x35"
"\x02\x0c\x29\xff\x4c\x9f\x91\xb1\x06\x50\x8e\xb0\xdb\x6b\x02\x0c\x09\x63\xf6\xa0\x91\x3f\x89\x0c\x34\x87"
"\x7e\x09";*/

WONCryptMini::ElGamal gKey;

void LoadVerifierKey()
{
	bool   aRet = false;
	WONCommon::RegKey aKey(REG_CONST::REG_TITAN_KEY_NAME, HKEY_LOCAL_MACHINE);

	// If key is open, read verifier key and update AuthInfo
	if (aKey.IsOpen())
	{
		unsigned long  aKeyLen = 0;
		unsigned char *aKeyP;
		if ((aKey.GetValue(REG_CONST::REG_AUTHVERIFIER_NAME, aKeyP, aKeyLen) == WONCommon::RegKey::Ok) &&
		    (aKeyLen > 0))
		{
			gKey.SetPublicKey(aKeyP,aKeyLen);
		}
		delete aKeyP;
	}


}

// g^x = h (mod p)
//
//				{ g*y, 0     < y < p/3
// Take f(y) =  | y*y, p/3   < y < 2*p/3
//				{ h*y, 2*p/3 < y < p
//
// y0 = g^k ->
//
// f(yi) = (g^ai)*(h^bi)
//
// Find yj = yi -> (g^ai)*(h^bi) == (g^aj)*(h^bj) -> g^(ai-aj) == h^(bj-bi) 
//
// -> g^[(ai-aj)/(bj - bi)] = h
//
// -> x = (ai - aj)*[(bj - bi)^-1] (mod p)
//

WONCryptMini::BigInteger ChineseRemainder(const std::vector<WONCryptMini::BigInteger> &r, const std::vector<WONCryptMini::BigInteger> &p)
{
	_ASSERT(r.size()==p.size());

	WONCryptMini::BigInteger x(0);


	WONCryptMini::BigInteger P(1);

	for(int i=0; i<p.size(); i++)
		P = P.mul(p[i]);


	for(int i=0; i<r.size(); i++)
	{
		WONCryptMini::BigInteger a = P.div(p[i]);
		WONCryptMini::BigInteger t = a.inverseModN(p[i]);

		x = x.add(a.mul(t).mul(r[i]));
	}

	x = x.mod(P);

	return x;
}


typedef __int64 FType;
void Factor(FType x, std::vector<WONCryptMini::BigInteger>&thePrimePowers, std::vector<WONCryptMini::BigInteger>&thePrimes, std::vector<int> &theMults)
{
	std::map<FType,FType> mFactorMap;
	std::map<FType,FType>::iterator anItr;

	while(x>1)
	{
		FType max = sqrt(x);

		bool foundFactor = false;
		for(FType i=2; i<=max; i++)
		{
			if(x%i==0)
			{
				anItr = mFactorMap.find(i);
				if(anItr==mFactorMap.end())
					mFactorMap[i] = 1;
				else
					anItr->second++;

				x/=i;

				foundFactor = true;
				break;
			}
		}
		
		if(!foundFactor)
		{
			anItr = mFactorMap.find(x);
			if(anItr==mFactorMap.end())
				mFactorMap[x] = 1;
			else
				anItr->second++;

			break;
		}
	}

	anItr = mFactorMap.begin();
	while(anItr!=mFactorMap.end())
	{
		thePrimePowers.push_back(WONCryptMini::BigInteger(anItr->first).exp(anItr->second));
		thePrimes.push_back(WONCryptMini::BigInteger(anItr->first));
		theMults.push_back(anItr->second);
		anItr++;
	}
}

void Factor(const WONCryptMini::BigInteger &x1, std::vector<WONCryptMini::BigInteger>&thePrimes, std::vector<int> &theMults)
{
	std::map<WONCryptMini::BigInteger,int> aFactorMap;
	std::map<WONCryptMini::BigInteger,int>::iterator anItr;

	WONCryptMini::BigInteger x = x1;

	while(x.bitLength()>8*sizeof(FType)-1)
	{
		std::cout << "x = " << x << std::endl;
		std::cout << "bitlength = " << x.bitLength() << std::endl;
		WONCryptMini::BigInteger max = x.sqrt();

		bool foundFactor = false;
		for(WONCryptMini::BigInteger i=2; i.cmp(max)<=0; i.add(1))
		{
			if(x.mod(i).cmp(WONCryptMini::BigInteger::ZERO)==0)
			{
				anItr = aFactorMap.find(i);
				if(anItr==aFactorMap.end())
					aFactorMap[i] = 1;
				else
					anItr->second++;

				x = x.div(i);

				foundFactor = true;
				break;
			}
		}
		
		if(!foundFactor)
		{
			anItr = aFactorMap.find(x);
			if(anItr==aFactorMap.end())
				aFactorMap[x] = 1;
			else
				anItr->second++;

			break;
		}
	}

	std::vector<WONCryptMini::BigInteger> primePowers, primeFactors;
	std::vector<int> multiplicity;

	Factor(x.GetInt64(),primePowers,primeFactors,multiplicity);
	for(int i=0; i<primeFactors.size(); i++)
	{
		anItr = aFactorMap.find(primeFactors[i]);
		if(anItr==aFactorMap.end())
			aFactorMap[primeFactors[i]] = 1;
		else
			anItr->second++;
	}


	anItr = aFactorMap.begin();
	while(anItr!=aFactorMap.end())
	{
		thePrimes.push_back(WONCryptMini::BigInteger(anItr->first));
		theMults.push_back(anItr->second);
		anItr++;
	}
}

WONCryptMini::BigInteger DiscreteLog(const WONCryptMini::BigInteger &y, const WONCryptMini::BigInteger &g, const WONCryptMini::BigInteger &p,
                                     const std::vector<WONCryptMini::BigInteger> &primeFactors,
                                     const std::vector<int> &multiplicity)
{

	std::vector<WONCryptMini::BigInteger> primePowers, r;

	for(int i=0; i<primeFactors.size(); i++)
		primePowers.push_back(primeFactors[i].exp(multiplicity[i]));

	for(int i=0; i<primeFactors.size(); i++)
	{
		WONCryptMini::BigInteger x = Hellman(y,g,p,primeFactors[i],multiplicity[i]);
		std::cout << "x = " << x << std::endl;
		r.push_back(x);
	}


	WONCryptMini::BigInteger result = ChineseRemainder(r,primePowers);
	return result;
}

WONCryptMini::BigInteger DiscreteLog(const WONCryptMini::BigInteger &y, const WONCryptMini::BigInteger &g, const WONCryptMini::BigInteger &p)
{
	std::vector<WONCryptMini::BigInteger> primeFactors;
	std::vector<int> multiplicity;

	Factor(p.sub(WONCryptMini::BigInteger::ONE),primeFactors,multiplicity);

	return DiscreteLog(y,g,p,primeFactors,multiplicity);
}


WONCryptMini::BigInteger DiscreteLog(const WONCryptMini::BigInteger &y, const WONCryptMini::BigInteger &g, const WONCryptMini::BigInteger &p, const WONCryptMini::BigInteger &q)
{
	std::vector<WONCryptMini::BigInteger> primeFactors;
	std::vector<int> multiplicity;

	Factor(p.sub(WONCryptMini::BigInteger::ONE).div(q),primeFactors,multiplicity);

	primeFactors.push_back(q);
	multiplicity.push_back(1);

	return DiscreteLog(y,g,p,primeFactors,multiplicity);
}