//
//  main.c
//  BLM209_Proje_3_Rework
//
//  Created by Elif Naz Gürerman and Yiğit Erdinç on 18.12.2021.
//

#include <stdio.h>
#include <stdlib.h>

struct Ucak {
	int oncelik_id;
	int ucak_id;
	int talep_edilen_inis_saati;
	int ertelenme_sayisi;
};

struct Ucak ucaklar[28];
struct Ucak inis_pisti_kullanim_sirasi[28];

char temp_oncelik_id[10];
char temp_ucak_id[8];
char temp_talep_edilen_inis_saati[23];
char ucak_turu[4][17] = {"ambulans ucagı", "savas ucagı", "yolcu ucagı", "kargo ucagı"};

void inputOku(void) {
	FILE *inputDosyasi;

	inputDosyasi = fopen("input.txt", "r");

	if (inputDosyasi == NULL) {
		printf("\nInput dosyası boş ya da bulunamadı.\n");
	} else {
		int i = -1;
		while (fscanf(inputDosyasi, "%s %s %s", temp_oncelik_id, temp_ucak_id, temp_talep_edilen_inis_saati)) {
			if (i == -1) {
				i++;
			} else if (i <= 27) {
				ucaklar[i].oncelik_id = atoi(temp_oncelik_id);
				ucaklar[i].ucak_id = atoi(temp_ucak_id);
				ucaklar[i].talep_edilen_inis_saati = atoi(temp_talep_edilen_inis_saati);
				i++;
			} else {
				break;
			}
		}
		fclose(inputDosyasi);
	}
}

void rotarliUcagiIndir(struct Ucak ucak) {
	// Sıraya almadığımız bu uçağın iniş saatini 1 saat arttırıp tekrar indirmeyi deniyoruz.
	ucak.ertelenme_sayisi++;
	for (; ucak.ertelenme_sayisi <= 3; ucak.ertelenme_sayisi++) {
		ucak.talep_edilen_inis_saati = ucak.talep_edilen_inis_saati + ucak.ertelenme_sayisi;
		if (ucak.talep_edilen_inis_saati > 24) {
			printf("\nSaat %d'e inis izni isteyen %d numarali %sna gun bitimi nedeniyle inis izni verilememektedir.\n",
						 inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].talep_edilen_inis_saati,
						 ucak.ucak_id,
						 ucak_turu[ucak.oncelik_id - 1]);
			break;
		}
		// Eğer iniş saatini erteleğimiz uçağın girmek istediği yeni sıra boşsa direkt sıraya alıyoruz.
		if (inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].talep_edilen_inis_saati == 0) {
			inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].oncelik_id = ucak.oncelik_id;
			inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].ucak_id = ucak.ucak_id;
			inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].talep_edilen_inis_saati = ucak.talep_edilen_inis_saati;
			inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].ertelenme_sayisi = ucak.ertelenme_sayisi;
			printf("\nSaat %d'e inis izni isteyen %d numarali %sna inis izni verilmistir.\n",
						 ucak.talep_edilen_inis_saati,
						 ucak.ucak_id,
						 ucak_turu[ucak.oncelik_id - 1]);
			break;
		} else {
			if (inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].oncelik_id <= ucak.oncelik_id) {
				// Değilse, sıraya girmek isteyen uçak ile sıradaki uçağın önceliğini kıyaslıyoruz.
				// Eğer sıraya girmek isteyen önceliği aynı yada daha küçükse sıraya almıyoruz.
				if (ucak.ertelenme_sayisi == 2) {
					// Eğer ki uçağın iniş saati 3 kereden fazla ertelenirse uçağı başka havalimanına yönlendiriyoruz.
					printf("\nSaat %d'e inis izni isteyen %d numarali %sna %d numarali %snin onceligi nedeniyle inis izni verilememektedir. Inisiniz daha fazla ertelenememektedir, lutfen baska havalimanina ilerleyiniz.\n",
								 inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].talep_edilen_inis_saati,
								 ucak.ucak_id,
								 ucak_turu[ucak.oncelik_id - 1],
								 inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].ucak_id,
								 ucak_turu[inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].oncelik_id - 1]);
					break;
				} else {
					// Uçağın iniş saati 3 kereden fazla ertelenmedi ise döngü devam ediyor.
					printf("\nSaat %d'e inis izni isteyen %d numarali %sna %d numarali %snin onceligi nedeniyle inis izni verilememektedir.\n",
								 inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].talep_edilen_inis_saati,
								 ucak.ucak_id,
								 ucak_turu[ucak.oncelik_id - 1],
								 inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].ucak_id,
								 ucak_turu[inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].oncelik_id - 1]);
				}
			} else if (inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].oncelik_id > ucak.oncelik_id) {
				if (inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].oncelik_id != 0) {
					struct Ucak tempUcak = inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati];
					inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].oncelik_id = ucak.oncelik_id;
					inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].ucak_id = ucak.ucak_id;
					inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].talep_edilen_inis_saati = ucak.talep_edilen_inis_saati;
					inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].ertelenme_sayisi = ucak.ertelenme_sayisi;
					rotarliUcagiIndir(tempUcak);
				} else {
					inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].oncelik_id = ucak.oncelik_id;
					inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].ucak_id = ucak.ucak_id;
					inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].talep_edilen_inis_saati = ucak.talep_edilen_inis_saati;
					inis_pisti_kullanim_sirasi[ucak.talep_edilen_inis_saati].ertelenme_sayisi = ucak.ertelenme_sayisi;
				}
				break;
			}
		}
	}
}

void ucaklariIndir(void) {
	for (int i = 0; i < 28; i++) {
		// Eğer uçağın girmek istediği sıra boşsa uçağı direkt sıraya alıyoruz.
		if (inis_pisti_kullanim_sirasi[ucaklar[i].talep_edilen_inis_saati].talep_edilen_inis_saati == 0) {
			inis_pisti_kullanim_sirasi[ucaklar[i].talep_edilen_inis_saati].oncelik_id = ucaklar[i].oncelik_id;
			inis_pisti_kullanim_sirasi[ucaklar[i].talep_edilen_inis_saati].ucak_id = ucaklar[i].ucak_id;
			inis_pisti_kullanim_sirasi[ucaklar[i].talep_edilen_inis_saati].talep_edilen_inis_saati = ucaklar[i].talep_edilen_inis_saati;
			inis_pisti_kullanim_sirasi[ucaklar[i].talep_edilen_inis_saati].ertelenme_sayisi = ucaklar[i].ertelenme_sayisi;
			printf("\nSaat %d'e inis izni isteyen %d numarali %sna inis izni verilmistir.\n",
						 ucaklar[i].talep_edilen_inis_saati,
						 ucaklar[i].ucak_id,
						 ucak_turu[ucaklar[i].oncelik_id - 1]);
		} else {
			// Değilse, sıraya girmek isteyen uçak ile sıradaki uçağın önceliğini kıyaslıyoruz.
			// Eğer sıraya girmek isteyen önceliği aynı yada daha küçükse sıraya almıyoruz.
			if (inis_pisti_kullanim_sirasi[ucaklar[i].talep_edilen_inis_saati].oncelik_id <= ucaklar[i].oncelik_id) {
				printf("\nSaat %d'e inis izni isteyen %d numarali %sna %d numarali %snin onceligi nedeniyle inis izni verilememektedir.\n",
							 inis_pisti_kullanim_sirasi[ucaklar[i].talep_edilen_inis_saati].talep_edilen_inis_saati,
							 ucaklar[i].ucak_id,
							 ucak_turu[ucaklar[i].oncelik_id - 1],
							 inis_pisti_kullanim_sirasi[ucaklar[i].talep_edilen_inis_saati].ucak_id,
							 ucak_turu[inis_pisti_kullanim_sirasi[ucaklar[i].talep_edilen_inis_saati].oncelik_id - 1]);

				rotarliUcagiIndir(ucaklar[i]);
			} else if (inis_pisti_kullanim_sirasi[ucaklar[i].talep_edilen_inis_saati].oncelik_id > ucaklar[i].oncelik_id) {

				struct Ucak ucak = 	inis_pisti_kullanim_sirasi[ucaklar[i].talep_edilen_inis_saati];

				inis_pisti_kullanim_sirasi[ucaklar[i].talep_edilen_inis_saati].oncelik_id = ucaklar[i].oncelik_id;
				inis_pisti_kullanim_sirasi[ucaklar[i].talep_edilen_inis_saati].ucak_id = ucaklar[i].ucak_id;
				inis_pisti_kullanim_sirasi[ucaklar[i].talep_edilen_inis_saati].talep_edilen_inis_saati = ucaklar[i].talep_edilen_inis_saati;
				inis_pisti_kullanim_sirasi[ucaklar[i].talep_edilen_inis_saati].ertelenme_sayisi = ucaklar[i].ertelenme_sayisi;

				rotarliUcagiIndir(ucak);
			}
		}
	}
	printf("\nTum ucaklar basariyla siralanmisti.\n");
}

void outputYaz(void) {
	FILE *outputDosyasi;

	outputDosyasi = fopen("output.txt", "w");

	fprintf(outputDosyasi, "oncelik_id, ucak_id, talep_edilen_inis_saati, inis_saati, gecikme_suresi, kalkis_saati\n");
	for (int i = 0; i <= 27; i++) {
		if (inis_pisti_kullanim_sirasi[i].oncelik_id != 0) {
			int kalkis_saati = inis_pisti_kullanim_sirasi[i].talep_edilen_inis_saati + inis_pisti_kullanim_sirasi[i].ertelenme_sayisi + 1;
			fprintf(outputDosyasi, "%d %d %d %d %d\n",
							inis_pisti_kullanim_sirasi[i].oncelik_id,
							inis_pisti_kullanim_sirasi[i].ucak_id,
							inis_pisti_kullanim_sirasi[i].talep_edilen_inis_saati,
							inis_pisti_kullanim_sirasi[i].ertelenme_sayisi,
							kalkis_saati);
		}
	}
	fclose(outputDosyasi);
}

int main(int argc, const char * argv[]) {

	printf("*** ISTANBUL HAVALIMANI KULE KONTROL SISTEMI ***\n");

	inputOku();

	ucaklariIndir();

	printf("\n\n*** SAATLERE GORE SIRALI INIS SIRASI ***\n");

	for (int i = 0; i <= 28; i++) {
		if (inis_pisti_kullanim_sirasi[i].oncelik_id != 0) {
			printf("\n%d %d %d",
						 inis_pisti_kullanim_sirasi[i].oncelik_id,
						 inis_pisti_kullanim_sirasi[i].ucak_id,
						 inis_pisti_kullanim_sirasi[i].talep_edilen_inis_saati);
		}
	}

	printf("\n\n\n*** SAATLERE GORE SIRALI KALKIS SIRASI ***\n\n");

	for (int i = 0; i <= 27; i++) {
		if (inis_pisti_kullanim_sirasi[i].oncelik_id != 0) {
			int kalkis_saati = inis_pisti_kullanim_sirasi[i].talep_edilen_inis_saati + inis_pisti_kullanim_sirasi[i].ertelenme_sayisi + 1;
			printf("%d %d %d %d %d\n",
						 inis_pisti_kullanim_sirasi[i].oncelik_id,
						 inis_pisti_kullanim_sirasi[i].ucak_id,
						 inis_pisti_kullanim_sirasi[i].talep_edilen_inis_saati,
						 inis_pisti_kullanim_sirasi[i].ertelenme_sayisi,
						 kalkis_saati);
		}
	}

	outputYaz();

	return 0;
}
