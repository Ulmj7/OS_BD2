# БД2 пост — Teams «БД2 - Submissions and Reviews» сувагт (хуулж тавих текст)

> Баг бүр зөвхөн **нэг** пост хэрэглэнэ (Guide §2).
> Хавсаргах файлууд: `docs/IV-2-4_BD2_Report_final.pdf`, `docs/IV-2-4_Manual_final.pdf`. Шүүмжид хариулах бэлэн эхийг `docs/IV-2-4_Review_Response.md`-ээс хуулна.

**Гарчиг**

```
IV-2-4 – Kernel Data Structures
```

**Тайлбар**

```
Код: IV-2-4
Гишүүд: Б. Лхамрагчаа (B241940102), Г. Үлэмж (B232270805), Т. Буянтөгс (B231910034), Э. Төгсбаяр (B221910021), Ө. Чинзориг (B200900028), Б. Идэрбат (B242270075)
Төсөл: Kernel Data Structures (OS Concepts 10e, 3-р бүлэг, Programming Project 4) — UI: Kernel structure graph
Товч үр дүн: Номын Part I (4 өнгөтэй list_head жагсаалт) болон Part II (collatz module, start параметр)-ийн жинхэнэ module эх кодыг user-space shim дээр өөрчлөлтгүй ажиллуулж, make test-ээр (5,000 санамсаргүй үйлдэл, 1,254 ENOMEM байрлал, Collatz 27 → 112 ба 837799 → 525 зангилаа) шалгасан. ASan болон kmalloc == kfree бүртгэлээр shim тестэд санах ойн алдагдал илрээгүй. Граф нь user-space C хөдөлгүүрийн list_head төлөвийг харуулдаг. Өмнөх модулиудыг баг Arch Linux дээр ачаалж, 4 өнгө ба Collatz 27-ийн 112 мөрийг (оргил 9232) шалгасан. II-4-1 Ubuntu дээр анхдагч 25, 27, буруу 0-ийг шалгаж, IV-2-1 Xubuntu дээр ачаалж, IV-2-7 Ubuntu sandbox-д хөрвүүлсэн (тайлан 8.7–8.9). Эцсийн ASan + UBSan шалгалтаар 4 C багц, 9 сценарийн 229 хариу, 15 HTTP тест амжилттай болсон. 2026-11-25-нд шинэ хоёр модуль Arch Linux 7.3.4-arch1-2 дээр хөрвөж, хоёр удаагийн insmod/rmmod амжилттай дууссан. Хоёр давталтын dmesg-д color_list: freed 4 nodes, collatz start=27 үед 112 утга (оргил 9232, индекс 77) ба collatz: freed 112 nodes гарсан. Анхдагч 25, буруу 0-ийг шинэ хувилбараар бодит кернелд давтаагүй. Kmemleak/slabinfo, 200 удаагийн давталт, бодит кернелийн ENOMEM шалгалт үлдсэн.
GitHub: https://github.com/Ulmj7/OS_BD2
Хавсралт: IV-2-4_BD2_Report_final.pdf, IV-2-4_Manual_final.pdf
Туршиж үзэх: README-ийн Quick Start хэсэг (make → make test → make run → http://127.0.0.1:8080).
```
