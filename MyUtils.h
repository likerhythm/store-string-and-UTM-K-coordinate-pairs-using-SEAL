#ifndef UTILS_H
#define UTILS_H

#include "SEALTools.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;
using namespace seal;

extern int name_data_size;
extern int coord_data_size;


class MyUtils {
private:
	Evaluator* evaluator;
	Decryptor* decryptor;
	Encryptor* encryptor;
	BatchEncoder* encoder;
	GaloisKeys galois_key;
	RelinKeys relin_key;
public:
	MyUtils(SEALTools seal_tools);
	vector<int64_t> decode(Ciphertext& encrypted, int data_size);
	void print_vector(vector<int64_t> matrix, int print_size);
	Ciphertext multiply(Ciphertext& encrypted1, Ciphertext& encrypted2);
	Ciphertext add(Ciphertext& encrypted1, Ciphertext& encrypted2);
	Ciphertext rotate(Ciphertext& encrypted);
	vector<int64_t> calc_distance(Ciphertext& usr, Ciphertext& dst);
	vector<int64_t> string_to_ascii(string& input);
	template <typename T>
	Ciphertext make_ciphertext(vector<T> v);
	void save(unordered_map<Ciphertext*, Ciphertext*>& ciphertext_map);
	Ciphertext set_user_xy();
};

#endif // UTILS_H
