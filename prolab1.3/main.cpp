#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char	text[247];
char	ucak_turu[4][20] = {"Ambulans Ucagina", "Savas Ucagina", "Yolcu ucagina", "Kargo ucagina"};

int		saat = 1;
int		sayilar[84];
int 	oncelik_id[28];
int 	ucak_id[28];
int 	talep_edilen_inis_saati[28];
int		talep_edilen_inis_saati2[28];
int		inen_ucaklar_indisi[28];
int		bekleme_suresi[28];

void	input_txt();
void	input_sayilari(char	text[]);
void	ucak_inis_verilerini_duzenle();
void	ucak_inis_saatlerine_gore_sirala();
void	degistir(int *i, int *j);
void	inis_pisti_kullanim_sirasi();
void	inis_yapacak_ucak(int i);
void	output_txt();

int	main(){	
	input_txt();
	output_txt();
	return (0);
}

void	input_txt(){
	system("color cf"); //Arkaplani acik kirmizi(c) yazilari parlak beyaz(f)
	FILE * input_dosyasi;
	input_dosyasi = fopen("input.txt", "r");
	
	for (int i = 0 ; i < 247 ; i++)
		text[i] = fgetc(input_dosyasi); //input.txt nin icindeki tum karakterleri text dizisine aktariyorum 
	input_sayilari(text); // Buradaki sayilari input_sayilara aktariyorum !!!!!!!!!!! input.txt deki 3 tane sayi verisinin en saginda bosluk olmasin hata verir!!!!!!!!!!!!!!
	fclose(input_dosyasi);
}

void	input_sayilari(char	text[]){
	int a = 0;
	
	for (int i = 43 ; i < 247 ; i++) // "oncelik_id ucak_id talep_edilen_inis_saati" bu satirdan sonraki sayilari tek tek diziye aktar, eger ki bosluk veya alt satira gec gorursen yeni sayiyi al
	{
		sayilar[a] = 0;
		while (1){
			sayilar[a] = (text[i] - 48) + (sayilar[a] * 10); // atoi ile tum sayilari aliyoruz toplam 3*28 tane veri
			i++;
			if (text[i] == ' ' || text[i] == '\n' || i == 247)
				break;
		}
		a++;
	}
	ucak_inis_verilerini_duzenle();
}

void 	ucak_inis_verilerini_duzenle(){
	for (int i = 0, a = 0 ; i < 84 ; i+=3){ // Her ucagin bilgisi satir kolon iliskisine gore atanir, her satira 3 tane kolon oldugu icin i, i+1, i+2 verileri uygun kategoriye atanir.
		oncelik_id[a] = sayilar[i];
		ucak_id[a] = sayilar[i + 1];
		talep_edilen_inis_saati[a] = sayilar[i + 2];
		a++;
	}
	ucak_inis_saatlerine_gore_sirala();
}

void	ucak_inis_saatlerine_gore_sirala(){
	
	for (int m = 0 ; m < 28 ; m++){
		bekleme_suresi[m] = 0;		// her ucagin baslangicdaki bekleme suresine 0 atadik
		inen_ucaklar_indisi[m] = -1;// her ucagin baslangicdaki inis durumuna -1 atadik, -1 inmedigini 1 ise indigini belirleyecek
	}
	
	for (int i = 0 ; i < 28 ; i++) // Burada ucaklarin inecegi saate gore siraliyoruz
	{
		for (int j = i ; j < 28 ; j++){
			if (talep_edilen_inis_saati[i] > talep_edilen_inis_saati[j]){
				degistir(&oncelik_id[i], &oncelik_id[j]);
				degistir(&ucak_id[i], &ucak_id[j]);
				degistir(&talep_edilen_inis_saati[i], &talep_edilen_inis_saati[j]);
			}
		}
	}
	
	for (int i = 0 ; i < 28 ; i++) // Burada ayni saatte inecek olan ucaklarin onceligine gore siraliyoruz
	{
		for (int j = i ; j < 28 ; j++){
			if ((talep_edilen_inis_saati[i] == talep_edilen_inis_saati[j]) && (oncelik_id[i] > oncelik_id[j])){
				degistir(&oncelik_id[i], &oncelik_id[j]);
				degistir(&ucak_id[i], &ucak_id[j]);
				degistir(&talep_edilen_inis_saati[i], &talep_edilen_inis_saati[j]);
			}
		}
	}
	
	for (int i = 0 ; i < 28 ; i++) // burada talep_edilen_inis_saati2 gibi yedek bir dizi olusturuyoruz ana dizimiz surekli degisiyor olacak cunku
		talep_edilen_inis_saati2[i] = talep_edilen_inis_saati[i];
	inis_pisti_kullanim_sirasi();
}

void	degistir(int *a, int *b){
	int temp = *a; // Gelen satirlari degistirmek icin swap kullaniyoruz
	*a = *b;
	*b = temp;
}

void	inis_pisti_kullanim_sirasi(){
	printf("\t\tHAVALIMANI UCUS YONETIM SISTEMI\n");
	int i, j, indis;
	
	for (i = 0 ; i < 24 ; i++){
		int	min = 10; // baslangic icin oncelik degerinden daha buyuk bir deger girmek yeterli
		for (j = 0 ; j < 28 ; j++){
			if (inen_ucaklar_indisi[j] == 1) // inmis olan ucaklari es gecmemiz lazim !!!
				continue;
			if (talep_edilen_inis_saati[j] == saat) // o saatte inemeyen ucaklarin degeri egerki 3 ise bundan sonra oncelik farketmeksizin onlar inmeli !!!
			{
				if (bekleme_suresi[j] >= 3) {
					indis = j; // egerki ucagimiz en az 3 kere beklediyse artik onu indirmeye karar veriyoruz, digerlerinin onceligine bakmadan
					break;
			 }
				else if (oncelik_id[j] < min){
					min = oncelik_id[j]; //ayni saatte egerki 3 kere bekleyen yoksa onceligi en yuksek olan ucagin indis degerini aliyoruz
					indis = j;
				}
			}
		}
		
		int temp = indis;
		if (i != 23)
			inis_yapacak_ucak(indis); // Hangi ucak inecekse o saattaki verileriyle birlikte yazdiriyoruz
		
		if (i == 23) // egerki son ucaga kadar geldiysek bu son ucaktan sonra gelen ve bu ucagimizdan daha oncelikli bir ucak varsa o ucagi indiriyoruz
		{
			for (j = 0 ; j < 28 ; j++){
				if (inen_ucaklar_indisi[j] == 1) // inmis olan ucaklari es gecmemiz lazim !!!
					continue;
				if (oncelik_id[j] < min){
					min = oncelik_id[j];
					indis = j;
				}
			}
			inis_yapacak_ucak(indis);
			printf("%d numarali %s %d numarali ucagin acil inisinden dolayi inis izni verilememistir, Lutfen Sabiha gokcene ilerleyiniz.", ucak_id[i], ucak_turu[oncelik_id[i] - 1], ucak_id[indis]);
			printf("\n");}
			
		for (j = 0 ; j < 28 ; j++){
			if (talep_edilen_inis_saati[j] == saat){
				if (j != indis && j != temp){
					printf("%d numarali %s %d numarali ucagin inmesi sebebiyle izin verilememistir, Sabiha gokcene ilerleyiniz.\n", ucak_id[j], ucak_turu[oncelik_id[j] - 1], ucak_id[indis]);
					talep_edilen_inis_saati[j]++; // ayni saatte inemeyen ucaklarin talep ettikleri saati 1 saat ileriye aliyoruz
					bekleme_suresi[j]++; // 1 saat bekleyen ucaklarin bekleme surelerini arttiriyoruz
				}
			}
		}
		printf("-------------------------------------------------------------------------");
		inen_ucaklar_indisi[indis] = 1; // egerki ucagimiz indiyse onu bayragini kaldiriyoruz
		//usleep(400000);
		saat++;
		if (i == 23) // max kapasite doldu
			return;
	}
}

void	inis_yapacak_ucak(int i){
	printf("\n%d numarali %s saat %d icin inis izni verilmistir.\n", ucak_id[i], ucak_turu[oncelik_id[i] - 1], saat);
}

void	output_txt(){
	FILE *kalkis;
	kalkis = fopen("output.txt", "a");
	
	fprintf(kalkis, "Oncelik ID | Ucak ID | Talep edilen inis saati | inis saati | gecikme suresi | kalkis saati\n");

	for (int i = 0 ; i < 28 ; i++){
		if (inen_ucaklar_indisi[i] == 1)
			fprintf(kalkis, "%d	       %d		   %d.00		%d.00	     %d saat		%d.00\n", oncelik_id[i], ucak_id[i], (talep_edilen_inis_saati2[i]), (talep_edilen_inis_saati2[i] + bekleme_suresi[i])%24, bekleme_suresi[i], (talep_edilen_inis_saati2[i] + bekleme_suresi[i] + 1)%24);
		else
			fprintf(kalkis, "%d	       %d		   %d.00		izin yok	     izin yok	izin yok\n", oncelik_id[i], ucak_id[i], (talep_edilen_inis_saati2[i]), (talep_edilen_inis_saati2[i] + bekleme_suresi[i])%24, bekleme_suresi[i], (talep_edilen_inis_saati2[i] + bekleme_suresi[i] + 1)%24);
	}
	fclose(kalkis);
}

