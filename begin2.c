// Program bir dosyadaki ilk satırdaki {..., ..., ...} formatindaki veriyi okur
// Dosya ismi programa argüman olarak verilir (Örn: .\program.exe deneme.txt) varsayılan: 'veri.txt'
// Okunan veriler bir dairesel listeye alfabetik sıra ile eklenir
// Bağlı liste elemanlarının adresleriyle beraber değerleri yazılır

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// satırdan alınabilecek max karakter sayısı
#define MAX_CH 200
// isim ve soyisim max uzunluğu
#define MAX_AD 100


struct Node {
    char isim[MAX_AD];
    char soyisim[MAX_AD];
    int tel;
    struct Node* next;
};


struct Node* root = NULL;
char birinci[MAX_CH];


void enter_bekle( void ) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// bir struct Node* nesnesinin değerleri tanımlanır
void yerlestir(struct Node* eleman, const char* isim, const char* soyisim, int tel, struct Node* next) {
    strncpy(eleman->isim, isim, MAX_AD);
    strncpy(eleman->soyisim, soyisim, MAX_AD);
    eleman->tel = tel;
    eleman->next = next;
}


// `{isim, soyisim, tel}` şeklinde verilen veriyi ayırır
// dairesel bağlı listeye alfabetik düzen oluşacak şekilde ekler
// aynı tel numarası liste içinde mevcut ise bir şey yapmaz
struct Node* ekle(struct Node* root, char* veri) {
    veri = veri + 1; // ilk { karakteri sayılmaz

    char* isim = strtok(veri, ", ");
    char* soyisim = strtok(NULL, ", ");
    int tel = atoi(strtok(NULL, ", "));

    // listenin boş olma durumu
    if (root == NULL) {
        root = malloc(sizeof(struct Node));
        yerlestir(root, isim, soyisim, tel, root);
        return root;
    }


    // Telefon numarası listede var mı kontrolü
    struct Node* tail = root;
    if (tail->tel == tel) {
        fprintf(stderr, "Bu telefon numarasi zaten kayitli!\n");
        return root;
    }
    while (tail->next != root) {
        tail = tail->next;
        if (tail->tel == tel) {
            fprintf(stderr, "Bu telefon numarasi zaten kayitli!\n");
            return root;
        }
    }
    // hata olmaması durumunda en son, tail listenin son elemanı olur

    struct Node* iter = tail;
    do {
        // isim alfabetik olarak öndeyse eleman öne eklenir
        // değilse soyisim alfabetik olarak önde veya eşitse öne eklenir
        // iki şart da sağlanmazsa devam edilir
        int kontrol = strcmp(iter->next->isim, isim);
        if (kontrol > 0 ||
            (kontrol == 0 && strcmp(iter->next->soyisim, soyisim) >= 0))
        {
            struct Node* new = malloc(sizeof(struct Node));
            yerlestir(new, isim, soyisim, tel, iter->next);
            iter->next = new;

            if (new->next == root) return new;
            else return root;
        }

        iter = iter->next;
    } while (iter != tail);

    // iter tail'e yetiştiğinde hala return olmamış ise eleman sona eklenir
    struct Node* new = malloc(sizeof(struct Node));
    yerlestir(new, isim, soyisim, tel, root);
    tail->next = new;
    return root;
}



int oku(char* dosyaadi) {
    // Dosya okunur, 'birinci' isimli stringe satırlardaki veriler aktarılır
    FILE* dosya = fopen(dosyaadi, "r");

    if (dosya == NULL) {
        perror("Dosya acilirken sorun: fopen");
        return errno;
    }

    char* okuma;

    okuma = fgets(birinci, MAX_CH, dosya);
    if (okuma == NULL) {
        perror("Dosya okuma hatasi: fgets");
        return errno;
    }

    fclose(dosya);
    return 0;
}



// Dosyadan okunan verileri virgül ve boşluklardan ayırır
// Bağlı listeye ekler
void listele( void ) {
    char* iter = birinci;
    char* den = iter;

    while (den != NULL) {
        den = iter;
        den = strchr(den+1, ',');
        den = strchr(den+1, ',');
        den = strchr(den+1, ',');
        if (den) *den = '\0';
        root = ekle(root, iter);
        if (den) iter = den + 2;
    }
}



void serbest_birakma( void ) {
    if (root != NULL) {
        struct Node* iter = root->next;
        root->next = NULL;
        root = NULL;

        while (iter != NULL) {
            struct Node* temp = iter;
            iter = iter->next;
            free(temp);
        }
    }
}




int main(int argc, char **argv) {
    atexit(serbest_birakma);

    char* dosyaadi;

    if (argc == 1)
        dosyaadi = "veri.txt";
    else
        dosyaadi = argv[1];


    int okundu = 0;

    while (1) {
        printf("\x1b[2J\x1b[H");
        puts("---------- Menu ----------");
        puts("1. Verileri Al");
        puts("2. Tersine Cevir");
        puts("3. Elemanlari ve adreslerini yaz");
        puts("4. Cikis");
        printf("Secin: ");

        char input[3];
        long secim;
        int c;

        fgets(input, 3, stdin);

        if (input[0] == '\n') // boş girdi durumu
            continue;
        else if (input[1] != '\n') {
            while ((c = getchar()) != '\n' && c != EOF); // tampon temizleme
        }

        secim = strtol(input, NULL, 10);

        // Dosya Okuma - Liste Oluşturma
        if (secim == 1) {
            int okuma = oku(dosyaadi);
            if (okuma == 0) {
                okundu = 1;
                printf("Dosya Okundu.\n");
                listele();
                printf("Liste Olusturuldu.\n");
                enter_bekle();
            } else {
                fprintf(stderr, "Dosya Okunamadi.\n");
                enter_bekle();
            }
        }

        // Listeyi Tersine Çevirme
        else if (secim == 2) {
            if (!okundu) {
                fprintf(stderr, "Oncelikle dosyayi okumalisiniz.\n");
                enter_bekle();
                continue;
            }

            // ........................................
            enter_bekle();
        }

        // Liste Elemanlarını Yazdırma
        else if (secim == 3) {
            if (!okundu) {
                fprintf(stderr, "Oncelikle dosyayi okumalisiniz.\n");
                enter_bekle();
                continue;
            }

            else {
                printf("--- Kisiler ---\n\n");

                struct Node* iter = root;

                if (iter == NULL) {
                    fprintf(stderr, "Veri yok.\n");
                    enter_bekle();
                    continue;
                }

                do {
                    printf("Isim: %s\nSoyisim: %s\nTel No: %d\n\n",
                            iter->isim, iter->soyisim, iter->tel);
                    iter = iter->next;
                } while (iter != root);
            }

            enter_bekle();
        }

        // Güvenli Çıkış
        else if (secim == 4) {
            printf("Cikis Yapiliyor...\n");
            return 0;
        }

        else {
            fprintf(stderr, "Gecerli cevap bir cevap girmediniz.\n");
            enter_bekle();
        }
    }
}

