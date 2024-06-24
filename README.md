# **1. 서론**
1.1 연구 배경
많은 지도 앱에서 사용자의 편의를 위해 사용자가 원하는 장소를 즐겨찾기 목록으로 등록할 수 있는 기능을 제공한다. 하지만 즐겨찾기 기능을 악용할 경우 사용자의 거주지 또는 직장의 위치 등을 쉽게 유추할 수 있다. 본 연구에서는 이러한 문제를 해결하기 위해 즐겨찾기 기능에 동형암호를 적용하여 암호화된 상태로 장소의 정보를 저장하고 탐색하는 방법을 제시한다.
1.2 가정
문제를 단순화하기 위해 즐겨찾기 기능이 다음과 같이 구성되었다고 가정한다. 1) 장소의 좌표는 미터 단위의 UTM-K 좌표계 기준으로 저장된다. 2) 저장할 수 있는 좌표의 범위는 대한민국을 벗어나지 않는다. 2-1) 좌표 범위를 대한민국으로 한정하면 UTM-K 좌표계의 원점(38°00'00.0"N 127°30'00.0"E )을 기준으로 x좌표 범위는 -150,000 ~ +150,000이고 y좌표의 범위는 0 ~ +450,000라고 가정한다. 3) 저장되는 장소 이름의 길이는 10을 넘어가지 않고 영어로만 구성된다. 4) 별도의 데이터베이스를 사용하지 않는다.
2. 본론
2.1 해결법 제시
<주요 변수>
vector<Ciphertext> name_db: 장소의 이름을 저장할 저장소
vector<Ciphertext> coord_db: 장소의 좌표를 저장할 저장소
int db_size(전역): db에 저장된 데이터 수
int name_data_size: 이름 데이터에 할당할 암호문 수
int coord_data_size: 좌표 데이터에 할당할 암호문 수
2.1.1 저장소의 형태
즐겨찾기 항목을 저장하기 위한 저장소는 장소의 이름을 저장할 vector<Ciphertext> name_db와 장소의 좌표를 저장할 vector<Ciphertext> coord_db로 나뉜다.name_db는 장소의 이름을 아스키코드로 변환한 값을 저장하며 암호문 하나 당 한 글자를 저장한다. 즉, 데이터 하나 당 10개의 10개의 암호문을 할당 받는다(구현 단계에서는 계산 시간을 줄이기 위해 3글자로 제한한다). 예를 들어 장소의 이름이 ‘apple’이라면 name_db에 저장되는 형태는 요소의 각각이 암호화된 vector인 [97, 112, 112, 108, 101, 0, 0, 0, 0, 0]이다. coord_db는 좌표값의 각 자릿수에 해당하는 숫자를 저장한다. 각 숫자는 하나의 암호문에 저장된다. 좌표 값의 범위가 6자리 수를 넘어가지 않으므로, x,y좌표 쌍 하나 당 12개의 암호문을 할당 받는다. 예를 들어 좌표가 (123, 4567)이라면 vector에 저장되는 암호문 형태는 [0, 0, 0, 1, 2, 3, 0, 0, 4, 5, 6, 7]이다. 이러한 형태로 좌표 값을 저장하는 이유는 사용자와 장소 사이의 직선거리의 계산을 효과적으로 하기 위함이다. 이는 2.1.2에서 설명한다.
2.1.2 P의 설정
동형암호의 암호문은 암호화된 상태로 연산 된 후 각 메시지가 미리 정해진 P보다 큰 값을 가지면 modulo P 값을 가진다. 따라서 올바른 계산이 이루어지려면 연산 결과의 범위가 P를 넘어가지 않아야 한다. 또한 이후에 설명할 페르마의 소정리 계산 시간을 단축하기 위해선 P-1은 작은 값들로 소인수분해가 되어야 한다. 본 연구에서는 P의 값을 673로 설정하였고 그 이유는 P-1=672=2^5*3*7로 비교적 작은 수로 간단하게 소인수분해 되기 때문이다.
2.1.3 데이터의 저장
사용자는 장소의 이름과 장소의 x,y좌표를 공백을 기준으로 넘겨줌으로써 데이터를 저장할 수 있다. 구체적인 저장 방법은 다음과 같다. 1) 사용자가 보낸 이름과 좌표를 각각Ciphertext로 암호화한다. 2) 암호문을 name_db와 coord_db vector에 pushback한다.
2.1.4 데이터의 탐색
사용자는 장소의 이름을 입력하는 것으로 저장된 장소 중 원하는 장소의 정보를 얻을 수 있다. 구체적인 탐색 방법은 다음과 같다. 1) 사용자가 보낸 장소의 이름을 vector<Ciphertext>로 변환하고 db_size만큼 같은 vector를 이어 붙인다. 즉, vector가 [1, 2, 3]이고 db_size가 2라면 [1, 2, 3, 1, 2, 3]과 같은 vector를 만든다 2) name_db와 1)에서 생성한 vector<Ciphertext>를 sub한 결과를 sub_result 암호문에 저장한다. 동일한 이름을 가진 장소가 존재한다면 sub_result에서 해당 장소가 저장된 위치의 값은 모두 0이다. 4) 장소 데이터 하나에 할당된 암호문의 값을 모두 더하여 하나의 암호문으로 만든다. 예를 들어 sub_result의 결과 장소 2개가 [1, 2, 3, 0, 0, 0]이라는 vector<Ciphertext>로 저장되어 있다면 [6, 0]과 같이 하나의 암호문으로 값을 더하여 축소시킨다. 5) 각각의 값에 페르마의 소정리를 사용하여 0보다 큰 값은 1로 만들어 그 결과를 mod_result에 저장한다. 5) 모든 값이 1로 설정된 vector<Ciphertext>에서 mod_result를 뺀다. 그 결과를 filter에 저장한다. 6) 현재 filter는 db에 저장된 데이터 수만큼 축소되어 있으므로 다시 암호문의 개수만큼 연장하여 filter를 재구성한다. 7) coord_db와 filter를 곱하는 연산을 수행한다. 그 결과 원하는 장소의 좌표 정보만 남아있는 암호문이 생성된다.
4) 단계에서 암호문의 sum을 구함으로써 문자가 일부만 일치하는 경우 1로 변경되는 문제를 해결할 수 있다. 예를 들어 장소이름 두 개가 [1, 2, 3, 1, 3, 5]으로 저장되어 있을 때 sum을 하지 않으면 탐색 시 검색어를 123로 입력했을 때 그 결과 [1, 1, 1, 1, 0, 0]의 filter가 생성될 것이다. 이것은 올바른 좌표를 select하는 데에 문제가 된다. sum을 사용한다면 filter를 생성하기 이전에 sum = 0인 부분을 확정지음으로써 저장된 데이터의 위치를 확실하게 정할 수 있다.
2.1.5 데이터의 삭제
데이터의 삭제 과정은 2.1.4과정에서 5)를 건너뛰는 과정과 유사하므로 생략한다.
2.2 구현
2.2.1 환경 설정

parms = EncryptionParameters(scheme_type::bfv);
poly_modulus_degree = 16384; // 한 암호문에 16384개 메시지 담을 수 있음.
parms.set_poly_modulus_degree(poly_modulus_degree);
        parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
parms.set_plain_modulus(673);


시스템 환경설정을 진행할 때 plain_modulus의 값을 소수 673으로 설정한다. 앞서 각 암호문이 가질 수 있는 최대 값이 648이므로 이보다 크고, 페르마 소정리 적용 시 지수승 계산을 비교적 빠르게 수행할 수 있는 소수로 선택하였다.
2.2.2 페르마의 소정리 구현
P=673으로 설정한 이유는 곱하기 수를 줄이기 위함이다. 페르마의 소정리를 사용하면 a를 672번 곱해야 하지만 곱하기 수가 너무 많기 때문에 672=2^5*3*7이라는 것을 사용하여 곱하기 수를 줄여서 a^672을 구할 수 있다.

Ciphertext SEALTools::calc_mod(Ciphertext enc) {
for (int i = 0; i < 4; i++) {
cout << "log - 제곱" << endl;
evaluator->square_inplace(enc);
evaluator->relinearize_inplace(enc, relin_key);
print_cipher_v2(enc);
cout << "log - invariant_nois_budget:";
cout << decryptor->invariant_noise_budget(enc) << endl << endl;
}
cout << "log - 3제곱" << endl;
evaluator->exponentiate_inplace(enc, 3, relin_key);
evaluator->relinearize_inplace(enc, relin_key);
print_cipher_v2(enc);
cout << "log - invariant_nois_budget:";
cout << decryptor->invariant_noise_budget(enc) << endl << endl;

cout << "log - 7제곱" << endl;
evaluator->exponentiate_inplace(enc, 7, relin_key);
evaluator->relinearize_inplace(enc, relin_key);
print_cipher_v2(enc);
cout << "log - invariant_nois_budget:";
cout << decryptor->invariant_noise_budget(enc) << endl << endl;
return enc;
}

2.2 문제점
- 계산 시간
적절한 P를 선택하여 곱하기수를 줄였다고 하지만 하나의 데이터에 대해 페르마 소정리를 적용할 때 4분정도의 시간이 소요되므로 아직까지 계산시간이 느리다.
- 계산 오류
이유는 모르겠지만 페르마 소정리 계산 결과가 1이 아닌 2로 출력되는 경우가 있다. 이 경우 올바른 장소 select가 이루어질 수 없다 
- 좌표 사용의 문제
coord_db 형태로 좌표가 저장된 경우 바로 사용하기 어려울 수 있다. 따라서 좌표를 다른 수단으로 사용하기 위해서는 다른 전처리를 거쳐야 하는 문제가 있다.
3. 결론 및 느낀 점
이번 연구를 통해 동형암호의 원리와 그 활용방법에 대해 깊게 생각해볼 수 있었다. 문자열과 좌표 값의 쌍을 저장하는 문제를 해결하는 방법에 대해 생각했지만 국지적인 해결 방법이라는 생각이 든다. 동형암호는 일반 데이터와 계산 방법이 완전히 다르기에 계산을 효율적으로 하기 위한 자료구조를 생각하는 게 까다롭다고 느껴졌다. 이미 AI나 빅데이터 등에 사용되고 있는 기술인 만큼 보편화된 자료구조가 있을 것이라고 생각된다.




