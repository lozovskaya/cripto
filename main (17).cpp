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

vector<unsigned int> Mul(const vector<unsigned int> &a, const vector<unsigned int> &b, unsigned int p) {
    vector<unsigned int> result(a.size() + b.size() - 1);
    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < b.size(); ++j) {
            result[i + j] += a[i] * b[j] % p;
            result[i + j] %= p;
        }
    }
    return result;
}

vector<unsigned long long> binpow(const vector<unsigned long long> &a, unsigned long long n, unsigned long long p) {
  vector<unsigned long long> res = {1};
    vector<unsigned int> copy_a;
    for (int i = 0; i < a.size(); ++i) {
        copy_a.push_back(a[i]);
    }
  while (n) {
    if (n % 2 == 1){
      res = Mul(res, copy_a, p);
    }
    copy_a = Mul(copy_a, copy_a, p);
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
    //reverse(answer.begin(), answer.end());
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


vector<unsigned int> input_polym(int p) {
    string s;
    getline(cin, s);
    vector<unsigned int> f;
    string number;
    for (char c : s) {
        if (c == ' ') {
            int coef = std::stoll(number);
            if (coef < 0) {
              coef += p;
            }
            f.push_back(coef);
          number = "";
        } else {
          number += c;
        }
    }
    if (!number.empty()) {
      int coef = std::stoll(number);
        if (coef < 0) {
          coef += p;
        }
        f.push_back(coef);
    }
    return f;
}

vector<vector<unsigned int>> split(const vector<unsigned int> &message, int n) {
  vector<vector<unsigned int>> blocks;
  vector<unsigned int> v;
  for (auto item : message) {
    v.push_back(item);
    if (v.size() == n) {
      blocks.emplace_back(v);
      v.clear();
    }
  }
  if (!v.empty()) {
    blocks.emplace_back(v);
  }
  return blocks;
}

void print_poly(const vector<unsigned int>& fq) {
  for (size_t i = 0; i < fq.size(); ++i) {
    cout << fq[i] << " ";
  }
  cout << "\n";
}

void to_norm(vector<unsigned int> &f, unsigned int p) {
    if (f.back() == 1) {
      return;
    }
    unsigned int k = binpow(f.back(), p - 2, p);
    for (unsigned int& item : f) {
      item = item * k % p;
    }
  }

void change(vector<unsigned int> &pol, const vector<unsigned int> &f, unsigned int p) {
    int n = (int) f.size() - 1;
    while (pol.size() > n) {
      unsigned int k = pol.back();
      for (int i = 0; i < n; ++i) {
        pol[pol.size() - i - 2] += p - f[n - i - 1] * k % p;
        pol[pol.size() - i - 2] %= p;
      }
      pol.pop_back();
    }
}

main() {
    unsigned long long p;
    cin >> p;
    cin.ignore();
    vector<unsigned int> f = input_polym(p);
    to_norm(f, p);
    vector<unsigned int> g = input_polym(p);
    change(g, f, p);
    vector<unsigned int> k = input_polym(p);
    change(k, f, p);
    string m;
    getline(cin, m);
    auto meaning = parse(m, p);
    //reverse(meaning.begin(), meaning.end());
    auto blocks = split(meaning, (int) f.size() - 1);
    for (auto& bl : blocks) { 
        change(bl, f, p);
        unsigned int b = 1;
        vector<unsigned long long> gb = binpow(g, b, p);
        change(gb, f, p);
        vector<unsigned long long> gab = binpow(k, b, p);
        change(gab, f, p);
        print_poly(gb);
        auto answer = Mul(bl, gab, p);
        change(answer, f, p);
        print_poly(answer);
    }
}
