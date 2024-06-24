#pragma once
#include "SEALTools.h"


//������ �߰�
void SEALTools::save_data(vector<Ciphertext>& name_db, vector<Ciphertext>& coord_db,
	vector<Ciphertext>& new_name, vector<Ciphertext>& new_coord) {
	cout << "���� name_db: ";
	print_db(name_db);
	cout << "���� coord_db: ";
	print_db(coord_db);

	cout << "[save data]" << endl;
	name_db.insert(name_db.end(), new_name.begin(), new_name.end());
	coord_db.insert(coord_db.end(), new_coord.begin(), new_coord.end());
	db_size += 1;

	cout << "������ �Ϸ�Ǿ����ϴ�." << endl;
	cout << "���� �� name_db: ";
	print_db(name_db);
	cout << "���� �� coord_db: ";
	print_db(coord_db);
	/*cout << "��� �̸� ���:";
	cout << "name_db: ";
	print_db(name_db);
	for (int i = 0; i < name_db.size(); i++) {
		Plaintext decrypted_plaintext;
		decryptor->decrypt(name_db[i], decrypted_plaintext);
		cout << "decrypted_plaintext: " << decrypted_plaintext.to_string();
		if (i % 3 == 0) {
			cout << " ";
		}
		if (decrypted_plaintext[0] != 0) {
			char character = static_cast<char>(decrypted_plaintext[0]);
			cout << character;
		}
	}*/
	cout << endl;
}

//������ ã��
void SEALTools::find_data(vector<Ciphertext>& name_db, vector<Ciphertext>& coord_db, vector<int64_t>& name_len_vec) {
	cout << "find_data" << endl;
	cout << "ã�� ����� �̸��� �Է����ּ���("<< name_data_size <<"�� ����)" << endl;
	string target = "he";
	//cin >> target;

	vector<int64_t> target_vector = ascii_to_vector(target);
	extend_vector(target_vector);
	vector<Ciphertext> target_enc = vec_to_cipher(target_vector);
	vector<Ciphertext> mod_result = calc_mod_result(name_db, target_enc);

	// 1, 0 �ٲٱ�
	vector<Ciphertext> reverse_result = change_one_zero(mod_result);
	cout << "log - [reverse_result]" << endl;
	print_db(reverse_result);

	//sum_result Ȯ��
	vector<Ciphertext> reverse_result_extended;
	for (int i = 0; i < reverse_result.size(); i++) {
		for (int j = 0; j < coord_data_size; j++) {
			reverse_result_extended.push_back(reverse_result[i]);
		}
	}

	cout << "log - [reverse_result_extended]" << endl;
	print_db(reverse_result_extended);

	//coord_db�� ���ϱ�
	vector<Ciphertext> multi_result;
	for (int i = 0; i < reverse_result_extended.size(); i++) {
		evaluator->multiply_inplace(reverse_result_extended[i], coord_db[i]);
		multi_result.push_back(reverse_result_extended[i]);
	}
	cout << "log - [multi_result]" << endl;
	print_db(multi_result);

	//����ڿ��� ��� ����ϱ�
	send_result(multi_result, reverse_result_extended);
}

void SEALTools::send_result(vector<Ciphertext> multi_result, vector<Ciphertext> reverse_result) {
	int64_t sum = 0;
	for (int i = 0; i < multi_result.size(); i++) {
		Plaintext decrypted_plaintext;
		decryptor->decrypt(multi_result[i], decrypted_plaintext);
		int64_t decoded_value = stol(decrypted_plaintext.to_string());
		sum += decoded_value;
	}
	cout << endl;
	if (sum == 0) {
		cout << "�ش��ϴ� ��� �����Ͱ� �������� �ʽ��ϴ�." << endl;
	}
	else {
		cout << "�ش��ϴ� ����� ��ǥ�� ������ �����ϴ�." << endl;
		cout << "��ǥ���� �ùٸ��� ���� ��� ������ �߻��� ���� ���� �ֽ��ϴ�." << endl;	
		for (int i = 0; i < multi_result.size(); i++) {
			if (i == 0) {
				cout << "x��ǥ: ";
			}
			else if (i == 6) {
				cout << endl << "y��ǥ: ";
			}
			Plaintext value_plain_text, filter_plain_text;
			decryptor->decrypt(multi_result[i], value_plain_text);
			int64_t decoded_value = value_plain_text[0];
			decryptor->decrypt(reverse_result[i], filter_plain_text);
			int64_t filter = filter_plain_text[0];
			if (filter > 0) {
				cout << decoded_value;
			}
		}
		cout << endl;
	}

	//Plaintext decrypted_plaintext;
	//decryptor->decrypt(ci, decrypted_plaintext);
	//int64_t decoded_value = decrypted_plaintext[0];
}

vector<Ciphertext> SEALTools::change_one_zero(vector<Ciphertext> mod_result) {
	//0->1, 1->0 �ٲٱ�
	vector<int64_t> only_one_vec(db_size, 1);
	vector<Ciphertext> only_one_enc = vec_to_cipher(only_one_vec);
	cout << "only_one_enc:";
	print_db(only_one_enc);
	cout << "mod_result:";
	print_db(mod_result);

	vector<Ciphertext> reverse_result;
	for (int i = 0; i < only_one_enc.size(); i++) {
		evaluator->sub_inplace(only_one_enc[i], mod_result[i]);
		reverse_result.push_back(only_one_enc[i]);
	}

	return reverse_result;
}

//������ ����
void SEALTools::del_data(vector<Ciphertext>& name_db, vector<Ciphertext>& coord_db) {
	cout << "������ ����� �̸��� �Է����ּ���("<< name_data_size <<"�� ����)" << endl;
	string target = "he";
	//cin >> target;

	vector<int64_t> target_vector = ascii_to_vector(target);
	extend_vector(target_vector);
	vector<Ciphertext> target_enc = vec_to_cipher(target_vector);
	vector<Ciphertext> mod_result = calc_mod_result(name_db, target_enc);

	//sum_result for name Ȯ��
	vector<Ciphertext> mod_result_extended_name;
	for (int i = 0; i < mod_result.size(); i++) {
		for (int j = 0; j < name_data_size; j++) {
			mod_result_extended_name.push_back(mod_result[i]);
		}
	}

	//sum_result for coord Ȯ��
	vector<Ciphertext> mod_result_extended_coord;
	for (int i = 0; i < mod_result.size(); i++) {
		for (int j = 0; j < coord_data_size; j++) {
			mod_result_extended_coord.push_back(mod_result[i]);
		}
	}

	cout << "log - [mod_result_extended_coord]" << endl;
	print_db(mod_result_extended_coord);

	//name_db�� ���ϱ�
	for (int i = 0; i < mod_result_extended_name.size(); i++) {
		evaluator->multiply_inplace(name_db[i], mod_result_extended_name[i]);
	}
	//coord_db�� ���ϱ�
	for (int i = 0; i < mod_result_extended_coord.size(); i++) {
		evaluator->multiply_inplace(coord_db[i], mod_result_extended_coord[i]);
	}

	cout << "������ �Ϸ�Ǿ����ϴ�." << endl;
	cout << "log - name_db:";
	print_db(name_db);
	cout << "log - coord_db:";
	print_db(coord_db);
}

// target�� ascii �ڵ� vector�� �ٲٱ�
vector<int64_t> SEALTools::ascii_to_vector(string target) {
	vector<int64_t> target_vector = string_to_ascii(target);
	vector<int64_t> additional_vector(name_data_size - target_vector.size(), 0);
	target_vector.insert(target_vector.end(), additional_vector.begin(), additional_vector.end());
	return target_vector;
}

// vector�� data_cnt��ŭ �̾� ���̱�
void SEALTools::extend_vector(vector<int64_t> &target_vector) {
	vector<int64_t> temp = target_vector; // ���� ����
	for (int i = 0; i < db_size - 1; i++) {
		target_vector.insert(target_vector.end(), temp.begin(), temp.end());
	}
}

// a^(P-1) mod P = 1 ����(P�� �Ҽ�)
vector<Ciphertext> SEALTools::calc_mod_result(vector<Ciphertext>& name_db, vector<Ciphertext>& target_enc) {
	// name_db���� ciphertext�� sub�ϱ�
	vector<Ciphertext> sub_result_enc;
	Ciphertext temp;
	for (int i = 0; i < name_db.size(); i++) {
		evaluator->sub(name_db[i], target_enc[i], temp);
		sub_result_enc.push_back(temp);
	}
	
	vector<Ciphertext> sum_result;
	for (int i = 0; i < sub_result_enc.size() / name_data_size; i++) {
		for (int j = 1; j < name_data_size; j++) {
			evaluator->add_inplace(sub_result_enc[i * name_data_size], sub_result_enc[i * name_data_size + j]);
		}
		sum_result.push_back(sub_result_enc[i * name_data_size]);
	}

	//mod ���
	vector<Ciphertext> mod_result_enc;
	for (int i = 0; i < sum_result.size(); i++) {
		cout << "log - [" << i + 1 << "��° ���]" << endl;
		temp = calc_mod(sum_result[i]);
		mod_result_enc.push_back(temp);
	}

	cout << "log - [mod_result_enc]" << endl;
	print_db(mod_result_enc);

	return mod_result_enc;
}

Ciphertext SEALTools::calc_mod(Ciphertext enc) {

	for (int i = 0; i < 4; i++) {
		cout << "log - ����" << endl;
		evaluator->square_inplace(enc);
		evaluator->relinearize_inplace(enc, relin_key);
		print_cipher_v2(enc);
		cout << "log - invariant_nois_budget:";
		cout << decryptor->invariant_noise_budget(enc) << endl << endl;
	}
	cout << "log - 3����" << endl;
	evaluator->exponentiate_inplace(enc, 3, relin_key);
	evaluator->relinearize_inplace(enc, relin_key);
	print_cipher_v2(enc);
	cout << "log - invariant_nois_budget:";
	cout << decryptor->invariant_noise_budget(enc) << endl << endl;

	cout << "log - 7����" << endl;
	evaluator->exponentiate_inplace(enc, 7, relin_key);
	evaluator->relinearize_inplace(enc, relin_key);
	print_cipher_v2(enc);
	cout << "log - invariant_nois_budget:";
	cout << decryptor->invariant_noise_budget(enc) << endl << endl;

	return enc;
}

// ã�� ���� ����
vector<int64_t> SEALTools::find(Ciphertext &coord_db, Ciphertext &mod_result) {
	// result�� coord_db ���ϱ�
	Ciphertext find_result;
	evaluator->multiply(coord_db, mod_result, find_result);

	// coord_vector���� 0�� ���� remove�� ���� vector �� �ڷ� �̵� ��Ű��
	// 1�� ������ erase�� 0 ��� �����
	Plaintext find_plain_text;
	vector<int64_t> find_vector;
	decryptor->decrypt(find_result, find_plain_text);
	encoder->decode(find_plain_text, find_vector);

	auto it = remove(find_vector.begin(), find_vector.end(), 0);

	// ���Ϳ��� 'value'�� ������ ����
	find_vector.erase(it, find_vector.end());
	
	return find_vector;
}

// 0->1 1->0 �ٲٱ�
Ciphertext SEALTools::reverse_one_zero(Ciphertext &mod_result) {
	// ��� �޽����� 1�� ��ȣ�� ����
	size_t slot_count = encoder->slot_count();
	cout << "slot_count = " << slot_count << endl;
	vector<int64_t> plain_vector(slot_count, 1);

	Plaintext plain;
	encoder->encode(plain_vector, plain);
	Ciphertext only_one_ci;
	encryptor->encrypt(plain, only_one_ci);

	evaluator->sub(only_one_ci, mod_result, mod_result);

	return mod_result;
}