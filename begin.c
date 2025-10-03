// Program bir dosyadaki ilk iki satırda bulunan, virgüllerle ayrılmış sayıları okur
// Dosya ismi programa argüman olarak verilir (Örn: .\program.exe deneme.txt) varsayılan: 'veri.txt'
// Okunan sayılar iki linked list'te tutulur
// Birinci liste küçükten büyüğe doğru sıralanır
// İkinci liste bu kuralı bozmadan birinci listeye dahil edilir
// Bağlı liste elemanlarının adresleriyle beraber değerleri yazılır

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// satırlardan alınabilecek max karakter sayısı
#define MAX_CH 100


struct Node {
    int data;
    struct Node* next;
};


struct Node* bagliliste1 = NULL;
struct Node* bagliliste2 = NULL;
char birinci[MAX_CH], ikinci[MAX_CH];
int siralandi = 0, boyut = 0, boyut2 = 0;



void enter_bekle( void ) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void serbest_birakma( void ) {
    for (struct Node* iter = bagliliste1; iter;) {
        struct Node* tmp = iter;
        iter = iter->next;
        free(tmp);
    }
    if (!siralandi) {
        for (struct Node* iter = bagliliste2; iter;) {
            struct Node* tmp = iter;
            iter = iter->next;
            free(tmp);
        }
    }
    bagliliste1 = NULL; bagliliste2 = NULL;
}



void ekleSondan(struct Node** root, int veri) {
    if (*root == NULL) {
        *root = malloc(sizeof(struct Node));
        (*root)->data = veri;
        (*root)->next = NULL;
    } else {
        struct Node* last = *root;
        while (last->next != NULL) {
            last = last->next;
        }

        last->next = malloc(sizeof(struct Node));
        last = last->next;
        last->data = veri;
        last->next = NULL;
    }
}



void swap(struct Node* onceki, struct Node* simdiki, struct Node* sonraki) {
    // simdiki ile sonrakinin yerini değiştirip öncekini sonrakine bağlar
    simdiki->next = sonraki->next;
    sonraki->next = simdiki;
    if (onceki != NULL) {
        onceki->next = sonraki;
    }
}



int oku(char* dosyaadi) {
    // Dosya okunur, 'birinci' ve 'ikinci' isimli stringlere satırlardaki sayılar aktarılır
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

    okuma = fgets(ikinci, MAX_CH, dosya);
    if (okuma == NULL) {
        perror("Dosya okuma hatasi: fgets");
        return errno;
    }

    fclose(dosya);
    return 0;
}



void listele() {
    // Dosyadan okunan değerler bağlı listelere aktarılır
    // Önceden liste oluşturulduysa önce free edilir
    serbest_birakma();
    siralandi = 0;
    boyut = 0; boyut2 = 0;

    long eklenen;

    char *ptr;
    ptr = strtok(birinci, ",");


    while (ptr != NULL) {
        eklenen = strtol(ptr, &ptr, 10);
        ekleSondan(&bagliliste1, eklenen);
        boyut++;
        ptr = strtok(NULL, ",");
    }

    ptr = strtok(ikinci, ",");

    while (ptr != NULL) {
        eklenen = strtol(ptr, &ptr, 10);
        ekleSondan(&bagliliste2, eklenen);
        boyut2++;
        ptr = strtok(NULL, ",");
    }
}



void sirala( void ) {
    // bagliliste1'i sıralar (bubble sort)
    struct Node* onceki = NULL;
    struct Node* iter;

    for (int i = 1; i <= (boyut - 1); i++) {
        iter = bagliliste1;

        for (int j = 0; j < (boyut - i); j++) {
            if (iter->data > iter->next->data) {
                // listenin ilk elemanı ise başı kaybetmemek için
                if (onceki == NULL) {
                    bagliliste1 = iter->next;
                }

                // swap sonunda ilerleyen elemanla beraber iter de ilerlemiş olur
                swap(onceki, iter, iter->next);
            }
            else {
                iter = iter->next;
            }

            // 'onceki' boşsa listenin başını göstersin
            // değilse bir eleman ilerlesin
            if (onceki == NULL)
                onceki = bagliliste1;
            else
                onceki = onceki->next;
        }
        onceki = NULL;
    }
}



void digerine_kat( void ) {
    // ikinci listenin elemanları birinci listeye
    // küçükten büyüğe sırası bozulmadan yerleştirilir
    struct Node* eleman;

    for (int i = 0; i < boyut2; i++) {
        eleman = bagliliste2;
        bagliliste2 = bagliliste2->next;
        struct Node* iter = bagliliste1;

        if (iter->data > eleman->data) {
            bagliliste1 = eleman;
            eleman->next = iter;
            continue;
        }

        for (int j = 0; j < (boyut - 1); j++) {
            if (iter->next->data > eleman->data) {
                eleman->next = iter->next;
                iter->next = eleman;
                break;
            }
            else {
                iter = iter->next;
            }
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
        puts("1. Dosyayi oku");
        puts("2. Sirala ve Birlestir");
        puts("3. Elemanlari ve adreslerini yaz");
        puts("4. Cikis");
        printf("Secin: ");

        char input[3];
        char* ptr;
        long secim;
        int c;

        fgets(input, 3, stdin);

        if (input[0] == '\n') // boş girdi durumu
            continue;
        else if (input[1] != '\n') {
            while ((c = getchar()) != '\n' && c != EOF); // tampon temizleme
        }

        secim = strtol(input, &ptr, 10);

        if (secim == 1) {
            int okuma = oku(dosyaadi);
            if (okuma == 0) {
                okundu = 1;
                printf("Dosya Okundu.\n");
                listele();
                printf("Listeler olusturuldu.\n");
                enter_bekle();
            } else {
                fprintf(stderr, "Dosya Okunamadi.\n");
                enter_bekle();
            }
        }

        else if (secim == 2) {
            if (!okundu) {
                fprintf(stderr, "Oncelikle dosyayi okumalisiniz.\n");
                enter_bekle();
                continue;
            }
            if (siralandi == 1) {
                printf("Zaten tek listede sirali.\n");
                enter_bekle();
                continue;
            }
            sirala();
            digerine_kat();
            siralandi = 1;
            printf("Tek listede siralandi.\n");
            enter_bekle();
        }

        else if (secim == 3) {
            if (!okundu) {
                fprintf(stderr, "Oncelikle dosyayi okumalisiniz.\n");
                enter_bekle();
                continue;
            }

            if (siralandi) {
                puts("Birlesmis listenin bellekteki adresleri ve degerleri:");

                for (struct Node* iter = bagliliste1; iter; iter = iter->next) {
                    printf("%p: %d ", iter, iter->data);
                }

                enter_bekle();
            } else {
                puts("Birinci liste:");

                for (struct Node* iter = bagliliste1; iter; iter = iter->next) {
                    printf("%p: %d ", iter, iter->data);
                }

                puts("\nIkinci liste:");

                for (struct Node* iter = bagliliste2; iter; iter = iter->next) {
                    printf("%p: %d ", iter, iter->data);
                }

                enter_bekle();
            }
        }

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

