#pragma once
#include "SEALTools.h"

SEALTools::SEALTools() {
    init();
}

SEALTools::~SEALTools() {
    delete encoder;
    delete keygen;
    delete encryptor;
    delete decryptor;
    delete evaluator;
    delete context;
}

void SEALTools::init() {
    try{
        cout << "Start" << endl;
        cout << "---------init---------" << endl;
        parms = EncryptionParameters(scheme_type::bfv);
        poly_modulus_degree = 16384; // 한 암호문에 8192개 메시지 담을 수 있음.
        parms.set_poly_modulus_degree(poly_modulus_degree);
        parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
        parms.set_plain_modulus(673);
        
        context = new SEALContext(parms);

        keygen = new KeyGenerator(*context);

        secret_key = keygen->secret_key();
        keygen->create_public_key(public_key);
        keygen->create_relin_keys(relin_key);
        keygen->create_galois_keys(galois_key);

        encryptor = new Encryptor(*context, public_key);
        decryptor = new Decryptor(*context, secret_key);
        evaluator = new Evaluator(*context);        

        cout << "Enc/Eval/Dec prepared!" << endl;
        cout << "----------------------" << endl;
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

GaloisKeys SEALTools::get_galois_key() { return galois_key; }
RelinKeys SEALTools::get_relin_key() { return relin_key; }
Encryptor* SEALTools::get_encryptor() { return encryptor; }
Decryptor* SEALTools::get_decryptor() { return decryptor; }
Evaluator* SEALTools::get_evaluator() { return evaluator; }