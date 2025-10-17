// Program bir dosyanın satırlarındaki verileri okur
// Dosya ismi programa argüman olarak verilir (Örn: .\program.exe deneme.txt) varsayılan: 'veri.txt'
// Okunan veriler bir Çift yönlü dairesel listeye eklenir
// Bağlı liste elemanlarının adresleriyle beraber değerleri yazılır


// Dosyanın satırlarında
// ürünlerin adı, satış miktarı ve stok miktarları bulunacak
// Örnek: Biskevit, 10, 90

// Menüden Bağlı listeyi oluştur seçildiği zaman ürünler çift yönlü bağlı listeye
// en çok satılan ürün en başta olacak şekilde azalan sırada eklenecektir.

// Menüden Satış gir seçilince kullanıcıdan ürün adı ve satış miktarı alınacak,
// stok bitip bitmediği kontrölü yapılacak ve stokta varsa,
// satış ve stok miktarları güncellenerek satış işlemi yapılacak ve
// sonrasında ürünün bağlı listedeki sırasında bir değişiklik gerekirse
// değişiklik gerçekleştirilecektir.
// Bağlı listede ürünler her zaman satış miktarına göre azalan sırada olacaktır.

// Menüden Stok gir seçilince ürün adı ve stok miktarı kulllanıcıdan alınacak
// ve bağlı listede ürünün stok miktarı güncellenecektir.

// Her işlemden sonra bağlı listenin son durumu ekrana başılacaktır.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Satır başına alınabilecek max karakter
#define MAX_CH 100
// Max ürün ismi uzunluğu
#define MAX_ISIM 50



struct Node {
    struct Node* prev;

    char urunadi[MAX_ISIM];
    unsigned satis;
    unsigned stok;

    struct Node* next;
};


// root bir global değişken
struct Node* root = NULL;



void enter_bekle( void ) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}




void yerlestir(struct Node* eleman, char* urunadi, unsigned satis, unsigned stok, struct Node* prev, struct Node* next) {
    strncpy(eleman->urunadi, urunadi, MAX_ISIM);
    eleman->satis = satis;
    eleman->stok = stok;
    eleman->prev = prev;
    eleman->next = next;
}




// Gelen veriyi ayrıştırır ve root'a ekler
struct Node* ekle(struct Node* root, char* veri) {
    char* urunadi = strtok(veri, ", ");
    unsigned satis = strtoul(strtok(NULL, ", "), NULL, 10);
    unsigned stok = strtoul(strtok(NULL, " \n"), NULL, 10);


    // liste başta boş ise
    if (root == NULL) {
        root = malloc(sizeof(struct Node));
        yerlestir(root, urunadi, satis, stok, NULL, NULL);
        return root;
    }

    // yeni elemanın satışı listenin ilk elemanından çoksa
    if (satis > root->satis) {
        struct Node* new = malloc(sizeof(struct Node));
        yerlestir(new, urunadi, satis, stok, NULL, root);
        root->prev = new;
        return new;
    }

    struct Node* iter = root;

    // Döngü sonlandığında iter ya listenin sonunda olacak
    // ya da yeni satış iterden sonraki elemanın satışından büyük olacak
    while (iter->next != NULL && satis <= iter->next->satis) {
        iter = iter->next;
    }

    struct Node* new = malloc(sizeof(struct Node));
    yerlestir(new, urunadi, satis, stok, iter, iter->next);
    new->prev->next = new;
    if (new->next != NULL) new->next->prev = new;

    return root;
}




struct Node* sil(struct Node* root, struct Node* eleman) {
    // silinecek eleman ilk eleman mı kontrolü
    if (root == eleman) {
        eleman = root->next;
        if (eleman) eleman->prev = NULL;
        free(root);
        return eleman;
    }

    // root'tan sonraki elemandan itibaren ara
    for (struct Node* iter = root->next; iter; iter = iter->next) {
        if (iter == eleman) {
            if (eleman->next)
                eleman->next->prev = eleman->prev;
            eleman->prev->next = eleman->next;
            free(eleman);
            return root;
        }
    }

    // eleman bulunamadıysa NULL dön
    return NULL;
}




// dosya okunur ve her satırdaki veri
// root'a ekle fonksiyonu ile eklenir
int oku(char* dosyaadi) {
    FILE* dosya = fopen(dosyaadi, "r");

    if (dosya == NULL) {
        perror("Dosya acilirken sorun: fopen");
        return errno;
    }


    char veri[MAX_CH];

    // Dosya sonuna veya tamamen boş bir satıra gelene kadar çalışır
    while (fgets(veri, MAX_CH, dosya) != NULL && (veri[0] != '\n' && veri[0] != '\r')) {
        root = ekle(root, veri);
    }

    fclose(dosya);
    return 0;
}



void liste_yazdir(struct Node* root) {
    if (root == NULL) {
        printf("Liste bos.\n");
        return;
    }

    while (root) {
        printf("Urun: %s\nStok: %u\nSatis: %u\n\n", root->urunadi, root->stok, root->satis);
        root = root->next;
    }
}




struct Node* bul(struct Node* head, char* urunadi) {
    for (struct Node* iter = head; iter; iter = iter->next) {
        if (strcmp(iter->urunadi, urunadi) == 0) {
            return iter;
        }
    }
    return NULL;
}




// Çift yönlü bağlı listeyi serbest bırakır
void serbest_birakma( void ) {
    if (!root) return;

    struct Node* iter = root;
    root = NULL;

    while (iter) {
        struct Node* temp = iter;
        iter = iter->next;
        free(temp);
    }
}





int main(int argc, char** argv) {
    atexit(serbest_birakma);

    char* dosyaadi = (argc > 1) ? argv[1] : "veri.txt";


    while (1) {
        printf("\x1b[2J\x1b[H");
        puts("---------- Menu ----------");
        puts("1. Bagli Listeyi Olustur");
        puts("2. Satis Gir");
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
            serbest_birakma();

            if (oku(dosyaadi) == 0) {
                printf("Dosya Okundu.\n");
                printf("Liste Olusturuldu.\n");
                liste_yazdir(root);
                enter_bekle();
            } else {
                fprintf(stderr, "Dosya Okunamadi.\n");
                enter_bekle();
            }
        }

        // Satis girme
        else if (secim == 2) {
            char urunadi[MAX_ISIM];
            unsigned int satis;

            printf("Urun adi girin: ");
            fgets(urunadi, MAX_ISIM, stdin);
            *strchr(urunadi, '\n') = '\0';

            printf("Satis miktari girin: ");
            scanf(" %u", &satis);
            while ((c = getchar()) != '\n' && c != EOF);

            struct Node* eleman = bul(root, urunadi);

            if (!eleman) {
                fprintf(stderr, "Girdiginiz urun listede bulunamadi.\n");
                enter_bekle();
                continue;
            }

            if (satis > eleman->stok) {
                fprintf(stderr, "Stok yetersiz!\n");
                enter_bekle();
                continue;
            }
            else {
                eleman->stok -= satis;
                eleman->satis += satis;

                char yeni_veri[MAX_CH];
                snprintf(yeni_veri, MAX_CH, "%s, %u, %u", eleman->urunadi, eleman->satis, eleman->stok);

                root = sil(root, eleman);
                eleman = NULL;
                root = ekle(root, yeni_veri);

                printf("%s urununden %u adet satildi.\n", urunadi, satis);
                liste_yazdir(root);
            }

            enter_bekle();
        }

        // Liste Elemanlarını Yazdırma
        else if (secim == 3) {
            liste_yazdir(root);
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

