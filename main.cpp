#include "SEALTools.h"

int name_data_size = 3;        // 이름이 차지하는 메시지 수
int coord_data_size = 12;       // 좌표가 차지하는 메시지 수
int db_size = 1;               // 저장된 데이터 수


vector<Ciphertext> SEALTools::vec_to_cipher(vector<int64_t> vec) {
	vector<Ciphertext> vec_ci;
	for (int i = 0; i < vec.size(); i++) {
		Ciphertext temp;
		Plaintext plaintext(to_string(vec[i]));
		encryptor->encrypt(plaintext, temp);
		vec_ci.push_back(temp);
	}
	return vec_ci;
}

void SEALTools::print_db(vector<Ciphertext> db) {
	cout << "log - ";
	for (int i = 0; i < db.size(); i++) {
		Plaintext decrypted_plaintext;
		decryptor->decrypt(db[i], decrypted_plaintext);
		int64_t decoded_value = stol(decrypted_plaintext.to_string());
		cout << decoded_value << " ";
	}
	cout << endl;
}

int main() {
	try {
		SEALTools seal_tools;
		cout << "SEALTools prepared!" << endl;


		vector<int64_t> name_vec = { 104, 101, 0 };
		vector<int64_t> coord_vec = { 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6 };
		vector<int64_t> name_len_vec = { 5 };

		vector<Ciphertext> name_db = seal_tools.vec_to_cipher(name_vec);
		vector<Ciphertext> coord_db = seal_tools.vec_to_cipher(coord_vec);
		cout << "name_db, coord_db prepared!" << endl;

		vector<int64_t> sample_name_vec = { 104, 105, 0};
		vector<int64_t> sample_coord_vec = { 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6 };
		vector<Ciphertext> sample_new_name = seal_tools.vec_to_cipher(sample_name_vec);
		vector<Ciphertext> sample_new_coord = seal_tools.vec_to_cipher(sample_coord_vec);

		seal_tools.save_data(name_db, coord_db, sample_new_name, sample_new_coord);
		seal_tools.find_data(name_db, coord_db, name_len_vec);
		seal_tools.del_data(name_db, coord_db);
	}
	catch (const exception& e) {
		cout << "Error: " << e.what() << endl;
	}
	return 0;
}
