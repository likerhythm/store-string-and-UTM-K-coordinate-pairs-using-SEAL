#ifndef METHOD_H
#define METHOD_H

#include "MyUtils.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;
using namespace seal;


extern int name_data_size;
extern int coord_data_size;
extern int db_size;

class MyMethod {
private:
	Evaluator* evaluator;
	BatchEncoder* encoder;
	Encryptor* encryptor;
	Decryptor* decryptor;

	RelinKeys relin_key;
	GaloisKeys galois_key;

	MyUtils utils;

	tuple<Ciphertext, Ciphertext> make_usr_ciphertext(string name, string x, string y);
	void get_usr_input(string& name, string& x, string& y);
	vector<int64_t> ascii_to_vector(string target);
	void extend_vector(vector<int64_t>& target_vector);
	Ciphertext calc_mod_result(Ciphertext& name_db, Ciphertext& target_enc);
	vector<int64_t> find(Ciphertext& coord_db, Ciphertext& mod_result);
	Ciphertext reverse_one_zero(Ciphertext& mod_result);
public:
	MyMethod(MyUtils utils, SEALTools seal_tools);
	void save_data(Ciphertext& name_db, Ciphertext& coord_db);
	vector<int64_t> find_data(Ciphertext& name_db, Ciphertext& coord_db);
	void del_data(Ciphertext& name_db, Ciphertext& coord_db);
};
#endif // METHOD_H
