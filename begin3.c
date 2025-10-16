// Program bir dosyanın satırlarındaki verileri okur
// Dosya ismi programa argüman olarak verilir (Örn: .\program.exe deneme.txt) varsayılan: 'veri.txt'
// Okunan veriler bir Çift yönlü dairesel listeye eklenir
// Bağlı liste elemanlarının adresleriyle beraber değerleri yazılır


// Dosyanın satırlarında
// ürünlerin adı, satış miktarı ve stok miktarları bulunacak
// Örnek: Biskevit, 10, 90

// Menüden Bağlı listeyi oluştur seçildiği zaman çift yönlü bağlı listeye
// ürünler en çok satılan ürün en başta olacak şekilde azalan sırada eklenecektir.

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
    int satis;
    int stok;

    struct Node* next;
};


// root bir global değişken
struct Node* root = NULL;



void enter_bekle( void ) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}




void yerlestir(struct Node* eleman, char* urunadi, int satis, int stok, struct Node* prev, struct Node* next) {
    eleman->urunadi = urunadi;
    eleman->satis = satis;
    eleman->stok = stok;
    eleman->prev = prev;
    eleman->next = next;
}



// Gelen veriyi ayrıştırır ve root'a ekler
struct Node* ekle(struct Node* root, char* veri) {
    char* urunadi = strtok(veri, ", ");
    int satis = atoi(strtok(NULL, ", "));
    int stok = atoi(strtok(NULL, " \n"));

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
        return new;
    }

    struct Node* iter = root;

    // Döngü sonlandığında iter ya listenin sonunda olacak
    // ya da yeni satış iterden sonraki elemanın satışından büyük olacak
    while (iter->next != NULL || satis <= iter->next->satis) {
        iter = iter->next;
    }

    struct Node* new = malloc(sizeof(struct Node));
    yerlestir(new, urunadi, satis, stok, iter, iter->next);
    new->prev->next = new;
    if (new->next != NULL) new->next->prev = new;

    return root;
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

    while (fgets(veri, MAX_CH, dosya) != NULL) {
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
        printf("Urun: %s\nStok: %d\nSatis: %d\n\n", root->urunadi, root->stok, root->satis);
        root = root->next;
    }
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
            if (oku(dosyaadi) == 0) {
                printf("Dosya Okundu.\n");
                printf("Liste Olusturuldu.\n");
                enter_bekle();
            } else {
                fprintf(stderr, "Dosya Okunamadi.\n");
                enter_bekle();
            }
        }

        // Satis girme
        else if (secim == 2) {
            ///////////////////////////////////////////////7
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

