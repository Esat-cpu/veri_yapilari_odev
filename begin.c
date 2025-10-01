// Program bir dosyadaki ilk iki satırda bulunan, virgüllerle ayrılmış sayıları okur
// Dosya ismi programa argüman olarak verilir (Örn: .\program.exe deneme.txt)
// Okunan sayılar iki linked list'te tutulur
// Birinci liste küçükten büyüğe doğru sıralanır
// İkinci liste bu kuralı bozmadan birinci listeye dahil edilir

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_CH 100


struct Node {
    int data;
    struct Node* next;
};


struct Node* root = NULL;
struct Node* root2 = NULL;
char birinci[MAX_CH], ikinci[MAX_CH];
int siralandi = 0;


void enter_bekle( void ) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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

int ekleIndexten(struct Node** root, unsigned index, int veri) {
    // ekleIndexten(&root, n, x) şeklinde kullanılmalı
    struct Node* node;

    if (index == 0) {
        node = malloc(sizeof(struct Node));
        node->data = veri;
        node->next = *root;
        *root = node;
    }
    else {
        struct Node* iter = *root;

        // iter, index'ten bir önceki liste elemanına gelir
        for (int i = 0; i < (index - 1); i++) {
            if (iter->next != NULL) {
                iter = iter->next;
            } else {
                fprintf(stderr, "Liste sınırı aşıldı.\n");
                return 1;
            }
        }

        node = malloc(sizeof(struct Node));
        node->data = veri;
        node->next = iter->next;
        iter->next = node;
    }

    return 0;
}


int oku(char* dosyaadi, char* birinci, char* ikinci) {
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
    int boyut = 0, boyut2 = 0;
    long eklenen;
    root = NULL;
    root2 = NULL;
    siralandi = 0;

    char *ptr;
    ptr = strtok(birinci, ",");

    while (ptr != NULL) {
        eklenen = strtol(ptr, &ptr, 10);
        ekleSondan(&root, eklenen);
        boyut++;
        ptr = strtok(NULL, ",");
    }

    ptr = strtok(ikinci, ",");

    while (ptr != NULL) {
        eklenen = strtol(ptr, &ptr, 10);
        ekleSondan(&root2, eklenen);
        boyut2++;
        ptr = strtok(NULL, ",");
    }
}


void serbest_birakma( void ) {
    for (struct Node* iter = root; iter;) {
        struct Node* tmp = iter;
        iter = iter->next;
        free(tmp);
    }
    if (!siralandi) {
        for (struct Node* iter = root2; iter;) {
            struct Node* tmp = iter;
            iter = iter->next;
            free(tmp);
        }
    }
}




int main(int argc, char **argv) {
    atexit(serbest_birakma);

    // Dosya Okuma İşlemleri
    if (argc == 1) {
        fprintf(stderr, "Dosya ismini arguman olarak verin!\n");
        return 1;
    }

    char* dosyaadi = argv[1];
    int okundu = 0;
    char birinci[MAX_CH], ikinci[MAX_CH];

    while (1) {
        printf("\x1b[2J\x1b[H");
        puts("---------- Menu ----------");
        puts("1. Dosyayi oku");
        puts("2. Sirala");
        puts("3. Elemanlari ve adreslerini yaz");
        puts("4. Cikis");
        printf("Secin: ");

        int secim, c;
        scanf("%d", &secim);
        while ((c = getchar()) != '\n' && c != EOF); // tampon temizleme

        if (secim == 1) {
            int okuma = oku(dosyaadi, birinci, ikinci);
            if (okuma == 0) {
                okundu = 1;
                printf("Dosya Okundu.\n");
                enter_bekle();
            } else {
                fprintf(stderr, "Dosya Okunamadi.\n");
                enter_bekle();
                continue;
            }
            listele();
        }
        else if (secim == 2) {
            if (!okundu) {
                fprintf(stderr, "Oncelikle dosyayi okumalisiniz.\n");
                enter_bekle();
                continue;
            }
            // ---
        }
        else if (secim == 3) {
            if (!okundu) {
                fprintf(stderr, "Oncelikle dosyayi okumalisiniz.\n");
                enter_bekle();
                continue;
            }
            if (siralandi) {
                puts("Birlesmis listenin bellekteki adresleri ve degerleri:");
                for (struct Node* iter = root; iter; iter = iter->next) {
                    printf("%p: %d ", iter, iter->data);
                }
                enter_bekle();
            } else {
                puts("Birinci liste:");
                for (struct Node* iter = root; iter; iter = iter->next) {
                    printf("%p: %d ", iter, iter->data);
                }
                puts("\nIkinci liste:");
                for (struct Node* iter = root2; iter; iter = iter->next) {
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
            fprintf(stderr, "Geçerli cevap bir cevap girmediniz.\n");
            enter_bekle();
        }
    }


   return 0;
}

