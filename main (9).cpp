#include <iostream>
#include <cstdlib>
#include <vector> 
#include <algorithm>
#include <string>
#include <cassert>

using namespace std;

#define int long long


struct biginteger {
    vector<unsigned int> number = {0};
    unsigned int group_b = 1e9;
    
    
    void delete_null() {
        while (number.size() > 1 && number.back() == 0) {
            number.pop_back();
        }
    }
    
    vector<unsigned int> add(const vector<unsigned int> &second) {
        unsigned int carry = 0;
        for (int i = 0; i < max(number.size(), second.size()) || carry; ++i) {
        	if (i == number.size()) {
        		number.push_back(0);
        	}
        	number[i] += carry + (i < second.size() ? second[i] : 0);
        	carry = number[i] >= group_b;
        	if (carry) {
        	    number[i] -= group_b;
        	}
        }
        return number;
    }
    
    vector<unsigned int> mult_by_a(unsigned long long a) {
        unsigned int carry = 0;
        for (int i = 0; i < number.size() || carry; ++i) {
        	if (i == number.size()) {
        		number.push_back(0);
        	}
        	unsigned long long cur = carry + (unsigned long long) number[i] * a;
        	number[i] = cur % group_b;
        	carry = cur / group_b;
        }
        delete_null();
        return number;
    }
    
    pair<vector<unsigned int>, unsigned int> divide(unsigned long long a) {
        unsigned int carry = 0;
        for (int i = (int) number.size() - 1; i >= 0; --i) {
        	unsigned long long cur = number[i] + (unsigned long long) carry * group_b;
        	number[i] = cur / a;
        	carry = cur % a;
        }
        delete_null();
        return make_pair(number, carry);
    }
};

int char_to_number(char symbol) {
  if (symbol >= 48 && symbol <= 57)
    return symbol - 48;
  if (symbol >= 65 && symbol <= 90)
    return symbol - 55;
  if (symbol >= 97 && symbol <= 122)
    return symbol - 61;
  if (symbol == 32)
    return 62;
  if (symbol == 46)
    return 63;
  return 64;
}

unsigned long long binpow(unsigned long long a, unsigned long long n, unsigned long long p) {
  unsigned long long res = 1;
  while (n) {
    if (n % 2 == 1){
      res *= a;
      res %= p;
    }
    a *= a;
    a %= p;
    n /= 2;
  }
  return res;
}


vector<unsigned int> from_a_to_b(const vector<unsigned int> &number, int a, int b) {
    biginteger answer10;
    // to 10
    for (int i = 0; i < number.size(); ++i) {
        answer10.mult_by_a(a);
        answer10.add({number[i]});
    }
    // to b
    vector<unsigned int> answer;
    while (answer10.number[0] != 0) {
        auto pp = answer10.divide(b);
        answer.push_back(pp.second);
    }
    reverse(answer.begin(), answer.end());
    return answer;
}

vector<unsigned int> parse(const string &s, unsigned int p) {
    vector<unsigned int> number;
    for (int i = 0; i < s.size(); ++i) {
        number.push_back(char_to_number(s[i]));
    }
    reverse(number.begin(), number.end());
    return from_a_to_b(number, 64, p);
}


main() {
    unsigned long long p, g, ga;
    cin >> p >> g >> ga;
    g %= p;
    ga %= p;
    string m;
    cin.ignore();
    getline(cin, m);
    auto meaning = parse(m, p);
    reverse(meaning.begin(), meaning.end());
    /*for (int i = 1; i < p; ++i) {
        cerr << i << ' ' << binpow(g, i, p) << endl;;
    }*/
    for (auto i : meaning) { 
        unsigned int b = rand() % (p - 1) + 1;
        unsigned long long gb = binpow(g, b, p);
        unsigned long long gab = binpow(ga, b, p);
        assert(i < p);
        assert(gb < p);
        assert(gab < p);
        cout << gb << ' ' << i * gab % p << endl;
    }
}
