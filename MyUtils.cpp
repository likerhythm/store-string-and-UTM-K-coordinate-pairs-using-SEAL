#include "SEALTools.h"


void SEALTools::print_cipher_v2(Ciphertext ci) {
	Plaintext decrypted_plaintext;
	decryptor->decrypt(ci, decrypted_plaintext);
	int64_t decoded_value = decrypted_plaintext[0];
	cout << "log - Decrypted value: " << decoded_value << endl;
}

vector<int64_t> SEALTools::calc_distance(Ciphertext& usr, Ciphertext& dst) {
	Ciphertext rotate_result;
	Ciphertext add_result;
	Ciphertext sub_result;
	Ciphertext ex_result;
	vector<int64_t> result_vector;

	// sub_result : [usr_x - dst_x1, usr_y - dst_y1, usr_x - dst_x2, usr_y - dst_y2]
	evaluator->sub(usr, dst, sub_result);
	cout << "sub completed!" << endl;

	// ex_result: [(usr_x - dst_x1)^2, (usr_y - dst_y1)^2, (usr_x - dst_x2)^2, (usr_y - dst_y2)^2]
	evaluator->exponentiate(sub_result, 2, relin_key, ex_result);
	cout << "exponential completed!" << endl;

	// add_result : [(usr_x - dst_x1)^2 + (usr_y - dst_y1)^2, .., (usr_x - dst_x2)^2 + (usr_y - dst_y2)^2, ..] => 짝수항만 표현
	evaluator->rotate_rows(ex_result, 1, galois_key, rotate_result);
	evaluator->add(ex_result, rotate_result, add_result);
	cout << "add completed!" << endl;

	cout << "계산된 거리는 다음과 같습니다." << endl;
	int i = 0;
	for (i = 0; result_vector[i] > 0; i += 2) {
		cout << result_vector[i] << endl;
	}

	return result_vector;
}
vector<int64_t> SEALTools::string_to_ascii(string& input) {
	vector<int64_t> ascii_values;
	for (char c : input) {
		ascii_values.push_back(static_cast<int64_t>(c));
	}
	return ascii_values;
}