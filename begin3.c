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



// Gelen veriyi ayrıştırır ve root'a ekler
struct Node* ekle(struct Node* root, char* veri) {

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



}

