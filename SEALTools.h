#ifndef SEALTOOLS_H
#define SEALTOOLS_H

#include "SEAL-4.1/seal/seal.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;
using namespace seal;

extern int name_data_size;        // 이름이 차지하는 메시지 수
extern int coord_data_size;       // 좌표가 차지하는 메시지 수
extern int db_size;               // 저장된 데이터 수

class SEALTools {
private:

    size_t poly_modulus_degree;
    EncryptionParameters parms;
    SEALContext* context;
    BatchEncoder* encoder;

    KeyGenerator* keygen;
    SecretKey secret_key;
    PublicKey public_key;
    GaloisKeys galois_key;
    RelinKeys relin_key;

    Encryptor* encryptor;
    Decryptor* decryptor;
    Evaluator* evaluator;

    vector<int64_t> ascii_to_vector(string target);
    void extend_vector(vector<int64_t>& target_vector);
    vector<Ciphertext> calc_mod_result(vector<Ciphertext>& name_db, vector<Ciphertext>& target_enc);
    vector<int64_t> find(Ciphertext& coord_db, Ciphertext& mod_result);
    Ciphertext reverse_one_zero(Ciphertext& mod_result);

    Ciphertext calc_mod(Ciphertext enc);
    vector<Ciphertext> change_one_zero(vector<Ciphertext> mod_result);
    void send_result(vector<Ciphertext> multi_result, vector<Ciphertext> reverse_result);
public:
	SEALTools();
    ~SEALTools();
	void init();

    GaloisKeys get_galois_key();
    RelinKeys get_relin_key();
    
    Encryptor* get_encryptor();
    Decryptor* get_decryptor();
    Evaluator* get_evaluator();

    //MyMethod
    void save_data(vector<Ciphertext>& name_db, vector<Ciphertext>& coord_db,
    vector<Ciphertext>& new_name, vector<Ciphertext>& new_coord);
    void find_data(vector<Ciphertext>& name_db, vector<Ciphertext>& coord_db, vector<int64_t>& name_len_vec);
    void del_data(vector<Ciphertext>& name_db, vector<Ciphertext>& coord_db);

    //MyUtils
    void print_cipher_v2(Ciphertext ci);
    vector<int64_t> calc_distance(Ciphertext& usr, Ciphertext& dst);
    vector<int64_t> string_to_ascii(string& input);

    vector<Ciphertext> vec_to_cipher(vector<int64_t> vec);
    void print_db(vector<Ciphertext> db);
};
#endif // SEALTOOLS_H