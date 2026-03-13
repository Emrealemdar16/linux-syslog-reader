# Linux Syslog Okuyucu - Çift Yönlü Bağlı Liste Uygulaması

Bu proje, Sistem Programlama dersi kapsamında Linux işletim sistemlerindeki sistem günlüklerini (syslog) okuyup, bu verileri bellek üzerinde **Çift Yönlü Bağlı Liste (Doubly Linked List)** veri yapısını kullanarak işleyen bir C uygulamasıdır. 

## 📌 Projenin Amacı
Sistem yöneticilerinin sıklıkla kullandığı log dosyalarını işleyebilecek algoritmik bir temel oluşturmak ve dinamik bellek yönetimi (dinamik diziler ve bağlı listeler) konularındaki yetkinliği göstermektir.

## 🧠 Teknik Analiz ve Soruların Cevapları

### Syslog Mekanizması Nedir ve Ne İçin Kullanılır?
Syslog, Linux/Unix sistemlerde çalışan servislerin, kernelin ve uygulamaların ürettiği olay, hata ve bilgi mesajlarını standart bir formatta toplayan mekanizmadır. Sistem sağlığını izlemek, güvenlik açıklarını tespit etmek ve meydana gelen hataları (debug) geçmişe dönük olarak incelemek amacıyla kullanılır.

### Bağlı Liste Kullanımı ve Seçimi
Bu projede log kayıtlarını tutmak için **Çift Yönlü Bağlı Liste (Doubly Linked List)** veri yapısı tercih edilmiştir. Syslog dosyasından okunan her satır, içerisinde metin, önceki düğüm göstericisi (`prev`) ve sonraki düğüm göstericisi (`next`) barındıran dinamik bir düğüme (Node) dönüştürülür.

**Neden Çift Yönlü Bağlı Liste?**
Log kayıtları doğası gereği zaman damgalıdır ve dosyaya eskiden yeniye doğru kronolojik yazılırlar. Bir sistemi incelerken genellikle **en son gerçekleşen güncel olayları ilk olarak görebilmek için** logları tersten (sondan başa) incelemek gerekir. Çift yönlü bağlı listeler, `prev` (önceki) pointer'ı sayesinde listenin kuyruğundan (tail) başlanarak ters kronolojik okuma yapılmasını en verimli şekilde sağlar.

## 🚀 Kurulum ve Çalıştırma

Proje dosyaları bilgisayara indirildikten sonra bir C derleyicisi (GCC vb.) ile derlenerek çalıştırılabilir. Windows ortamında test edilebilmesi için repoda örnek bir `syslog.txt` dosyası mevcuttur.

```bash
gcc main.c -o syslog_reader
./syslog_reader
