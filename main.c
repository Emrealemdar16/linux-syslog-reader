#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* * Çift Yönlü Baðlý Liste (Doubly Linked List) Düðüm Yapýsý
 * Her bir düðüm, metin dosyasýndan okunan bir satýrý ve
 * listedeki önceki/sonraki düðümlerin bellek adreslerini tutar.
 */
typedef struct SyslogNode {
    char log_message[1024];         // Okunan log satýrýný saklayacak karakter dizisi
    struct SyslogNode* prev;        // Bir önceki düðümü (daha eski log) gösteren iþaretçi
    struct SyslogNode* next;        // Bir sonraki düðümü (daha yeni log) gösteren iþaretçi
} SyslogNode;

/*
 * Yeni bir log düðümü oluþturur ve bellekte yer ayýrýr.
 * Parametre: Okunan log metni (message)
 */
SyslogNode* create_log_node(const char* message) {
    // malloc ile dinamik bellek tahsisi yapýyoruz
    SyslogNode* new_node = (SyslogNode*)malloc(sizeof(SyslogNode));
    if (new_node == NULL) {
        printf("Hata: Bellek tahsisi basarisiz oldu!\n");
        exit(1);
    }

    // Gelen mesajý güvenli bir þekilde (strncpy) düðümün içine kopyalýyoruz
    strncpy(new_node->log_message, message, sizeof(new_node->log_message) - 1);
    new_node->log_message[sizeof(new_node->log_message) - 1] = '\0'; // String sonlandýrýcý eklendi

    // Yeni düðümün komþularý henüz belli olmadýðý için NULL atýyoruz
    new_node->prev = NULL;
    new_node->next = NULL;

    return new_node;
}

/*
 * Oluþturulan yeni düðümü çift yönlü baðlý listenin sonuna (kuyruða) ekler.
 */
void append_log_node(SyslogNode** head, SyslogNode** tail, const char* message) {
    SyslogNode* new_node = create_log_node(message);

    // Eðer liste tamamen boþsa, ilk eleman hem head hem tail olur
    if (*head == NULL) {
        *head = new_node;
        *tail = new_node;
        return;
    }

    // Liste boþ deðilse, yeni düðümü mevcut kuyruðun (tail) sonuna baðlýyoruz
    (*tail)->next = new_node;      // Eski son elemanýn 'next'i yeni elemaný göstersin
    new_node->prev = *tail;        // Yeni elemanýn 'prev'i eski son elemaný göstersin
    *tail = new_node;              // Kuyruk (tail) iþaretçisini yeni eleman olarak güncelliyoruz
}

/*
 * Loglarý normal sýrayla (Eskiden Yeniye / Baþtan Sona) yazdýrýr.
 */
void print_logs_forward(SyslogNode* head) {
    printf("\n--- SYSLOG KAYITLARI (KRONOLOJIK: ESKIDEN YENIYE) ---\n");
    SyslogNode* current = head;
    while (current != NULL) {
        printf("%s", current->log_message);
        current = current->next; // Ýleriye doðru adým at
    }
    printf("----------------------------------------------------\n");
}

/*
 * Loglarý TERS sýrayla (Yeniden Eskiye / Sondan Baþa) yazdýrýr.
 * Çift yönlü baðlý liste (Doubly Linked List) kullanmamýzýn asýl amacý bu fonksiyondur.
 * Sistem yöneticileri her zaman en güncel loglarý ilk görmek ister.
 */
void print_logs_reverse(SyslogNode* tail) {
    printf("\n--- SYSLOG KAYITLARI (TERS KRONOLOJIK: YENIDEN ESKIYE) ---\n");
    SyslogNode* current = tail;
    // Kuyruktan (tail) baþlayýp 'prev' iþaretçileri ile geriye doðru gidiyoruz
    while (current != NULL) {
        printf("%s", current->log_message);
        current = current->prev; // Geriye doðru adým at
    }
    printf("----------------------------------------------------\n");
}

/*
 * Program sonlandýðýnda dinamik olarak ayrýlan belleði (RAM) sisteme iade eder.
 * Memory leak (bellek sýzýntýsý) oluþmasýný engeller.
 */
void free_linked_list(SyslogNode** head) {
    SyslogNode* current = *head;
    SyslogNode* next_node;

    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    *head = NULL;
}

int main() {
    SyslogNode* log_list_head = NULL; // Listenin baþlangýç noktasý (Head)
    SyslogNode* log_list_tail = NULL; // Listenin bitiþ noktasý (Tail)

    // Windows ortamýnda test edebilmek için oluþturduðumuz yerel metin dosyasý
    const char* file_path = "syslog.txt";

    // Dosyayý sadece okuma ("r") modunda açýyoruz
    FILE* file = fopen(file_path, "r");

    // Eðer dosya bulunamazsa verilecek hata mesajýný da güncelledik
    if (file == NULL) {
        printf("Hata: '%s' dosyasi ayni klasorde bulunamadi!\n", file_path);
        printf("Lutfen C kodu ile ayni klasorun icinde syslog.txt dosyasinin oldugundan emin olun.\n");
        return 1;
    }

    char line_buffer[1024];
    int log_count = 0;

    printf("Syslog dosyasi okunuyor ve Cift Yonlu Bagli Listeye aktariliyor...\n");

    // fgets ile dosyayý satýr satýr okuyup baðlý listeye ekliyoruz
    while (fgets(line_buffer, sizeof(line_buffer), file) != NULL) {
        append_log_node(&log_list_head, &log_list_tail, line_buffer);
        log_count++;
    }

    fclose(file); // Dosya ile iþimiz bitti, kapatýyoruz
    printf("Toplam %d adet log kaydi listeye eklendi.\n", log_count);

    // Loglarý en güncelden eskiye doðru (sondan baþa) yazdýrýyoruz
    print_logs_reverse(log_list_tail);

    // Ýþlem bitince belleði temizliyoruz
    free_linked_list(&log_list_head);

    return 0;
}
