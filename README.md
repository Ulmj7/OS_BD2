# Kernel Data Structures — IV-2-4

| | |
|---|---|
| **Хичээл** | F.CSM302 Үйлдлийн систем — Бие даалт 2 (БД2) |
| **Хуваарилалтын код** | **IV-2-4** |
| **Төсөл** | Kernel Data Structures (OS Concepts 10e, Chapter 3, Programming Project 4) |
| **Үндсэн UI дүрслэл** | Kernel structure graph (элемент нэмэх / устгах / хайх үйлдлийн дараах бүтцийг графаар харуулна) |
| **Гишүүд** | Б. Лхамрагчаа (B241940102) · Г. Үлэмж (B232270805) · Т. Буянтөгс (B231910034) · Э. Төгсбаяр (B221910021) · Ө. Чинзориг (B200900028) · Б. Идэрбат (B242270075) |
| **GitHub** | https://github.com/Ulmj7/OS_BD2 |

## Зорилго

Linux kernel-ийн дугуй (circular) хоёр чиглэлт холбоост жагсаалт `struct list_head`
(`<linux/list.h>`)-ийг сурах бичгийн даалгаврын дагуу хэрэгжүүлж, түүний үйлдлүүдийг
(`list_add_tail`, `list_for_each_entry`, `list_del`, `list_for_each_entry_safe`, `kmalloc`/`kfree`)
хөрвүүлэгдэх бодит кодоор турших, мөн **бодит ажиллаж буй төлөвөөс** зурагддаг интерактив граф
харуулах.

Сурах бичгийн шаардлага:

* **Part I** — `struct color {red, blue, green, list}` бүхий 4 элементтэй жагсаалт үүсгэж, dmesg-д
  хэвлэх; module-ээс гарахад `list_del` + `kfree`-ээр бүгдийг чөлөөлөх (`src/kernel/color_list.c`).
* **Part II** — `collatz` module: `start` параметр (default 25) авч, Collatz дарааллыг kernel
  жагсаалтад хадгалж, хэвлэж, гарахад чөлөөлөх (`src/kernel/collatz.c`).

> **Бодит кернел дээр.** 2026-11-25-нд шинэчилсэн `color_list.ko`, `collatz.ko`-г Arch Linux
> 7.3.4-arch1-2 x86_64 дээр `make module KDIR=/usr/lib/modules/$(uname -r)/build`-ээр хөрвүүлж,
> `insmod` → `rmmod` → `dmesg` дарааллаар хоёр удаа шалгасан. Өнгөний 4 мөрийн дараа
> `color_list: freed 4 nodes`, Collatz `start=27`-ийн 112 мөрийн (оргил 9232, индекс 77) дараа
> `collatz: freed 112 nodes` гарсан. Хоёр давталтын үр дүн ижил.
>
> Өмнөх хувилбарын анхдагч 25, 27, буруу 0-ийг II-4-1 Ubuntu дээр, ачаалалтыг IV-2-1 Xubuntu дээр
> шалгасан. IV-2-7 Ubuntu sandbox-д хөрвүүлсэн (тайлан 8.7–8.9, гарын авлага 8.2.7).
> Анхдагч 25 ба буруу 0-ийг шинэ хувилбараар бодит кернелд давтаагүй. ASan ба
> `kmalloc == kfree` бүртгэлээр shim тестэд алдагдал илрээгүй. Бодит кернелийн kmemleak/slabinfo,
> 200 удаагийн ачаалж/зайлуулах давталт, ENOMEM шалгалт үлдсэн.

## Quick Start

Шаардлага: Linux, `gcc`, `make`, `python3` (зөвхөн стандарт сан), орчин үеийн browser. Туршсан хувилбар: Arch Linux, gcc 16.2.1, GNU Make 4.4.1, Python 3.14.8. Одоогийн UI шалгалт Brave 158 дээр амжилттай.
Гаднаас сан суулгах, root эрх, интернет **шаардлагагүй**.

```sh
git clone --branch main https://github.com/Ulmj7/OS_BD2.git kernel-data-structures
cd kernel-data-structures
make            # build/kds_engine болон build/libkds.a-г бүтээнэ
make test       # user-space тест (make test-user-тэй адил): C, engine, HTTP; ASan/UBSan
make run        # UI-г http://127.0.0.1:8080 дээр асаана (зөвхөн локал)
```

Browser-ээр `http://127.0.0.1:8080` нээгээд **Color / Collatz / Stack / Queue** табуудаас үйлдэл
хийнэ. Граф нь user-space shim ашигладаг C хөдөлгүүрийн төлөвийг зурна. Граф болон «Хүссэн байт»
тоолуурт ачаалсан `.ko` модулийн төлөв, кернелийн slab хэмжээ орохгүй. Дэлгэрэнгүйг
`docs/IV-2-4_Manual_final.pdf`-аас үз. Жишээ нь URL-ээр автоматаар ажиллуулж болно:

```
http://127.0.0.1:8080/?run=color%20example;color%20delete%20index=1
```

Жинхэнэ Linux кернелд (кернелийн header-тэй машинд; дэлгэрэнгүй `docs/IV-2-4_Manual_final.pdf` 8.2.7):

```sh
make module KDIR=/usr/lib/modules/$(uname -r)/build     # src/kernel/color_list.ko, collatz.ko (src/kernel/Makefile, Kbuild)
sudo insmod src/kernel/collatz.ko start=27
sudo dmesg | tail -130
sudo rmmod collatz
```

Анхаар: кернел модуль ажиллаж буй кернелд ачаалагдана; ажлаа хадгалж, боломжтой бол виртуал машин ашиглана.

## Репозиторийн бүтэц

```
README.md
LICENSE                  GPL-2.0
Makefile                 make / make test / make test-user / make run / make module / make clean
src/shim/                user-space "linux/*.h" shim: list.h (өөрийн хэрэгжилт), kmalloc/kfree, printk, module_param
src/kernel/              БОДИТ кернел модулийн эх код: color_list.c, collatz.c, Kbuild Makefile
src/engine/              kds_core.{h,c}, main.c -> build/kds_engine (stdin команд -> нэг мөр JSON)
src/ui/server.py         python3 stdlib HTTP сервер (127.0.0.1), нэг тогтвортой engine процесс
src/ui/static/           index.html, app.js, style.css — SVG граф, офлайн, light/dark
tests/                   C, protocol, HTTP, UI шалгалт; README.md; PHASE2_REVIEW.md
video/                   IV-2-4_BD1_Demo.mp4 — танилцуулах видео
docs/                    IV-2-4_BD2_Report_final.pdf, IV-2-4_Manual_final.pdf, IV-2-4_Teams_Post.md, IV-2-4_Review_Response.md, img/ (бодит screenshot), src/ (PDF-ийн HTML эх)
```

## Тест

ASan + UBSan-тай шалгалтаар 4 C багц, 9 сценарийн 229 хариу, 15 HTTP тест амжилттай.
Үндсэн багцыг дараалан 5/5, HTTP багцыг 30/30 удаа амжилттай ажиллуулсан.

`make test` нь дараахыг ажиллуулна (үр дүн, хүлээгдсэн утга бүрийн гарал үүсэл — `tests/README.md`):

* `tests/test_list.c` — өөрийн `list.h`-ийн unit тест + seed-тэй санамсаргүй differential тест
* `tests/test_color_module.c`, `tests/test_collatz_module.c` — **жинхэнэ** module `.c`-г shim-тэй хөрвүүлж,
  `printk` гаралт, `kmalloc == kfree`, `kmalloc` бүрийн байрлалд ENOMEM тарьж leak шалгана
* `tests/test_core.c` — engine-ийн бүх команд, зориудаар эвдсэн жагсаалтыг `kds_verify` барьж буй эсэх
* `tests/run_cases.py` — `tests/cases/*.in` + хүлээгдсэн JSON (ном дахь жишээ, boundary/error кейсүүд)
* `tests/test_server.py` — HTTP сервер, 15 тест (400 кейс, path traversal, restart, loopback-only bind, Ctrl+C-ийн цэвэр shutdown, хэт том body, гадаад Host, 100 зэрэгцээ POST). Loopback тест IPv6 идэвхгүй Linux дээр ажиллана.
* `tests/ui/browser_check.py`, `tests/ui/interactions.py` — **заавал биш** Brave/Chromium (CDP) UI шалгалт; `make test`-д ороогүй (`make run`-ийн дараа гараар ажиллуулна, `tests/README.md`)

## Багийн гүйцэтгэл ба ашигласан хэрэгсэл

Эх код, тест болон баримт бичгийн нооргийг AI агентууд (Anthropic Claude, OpenAI Codex) ашиглан
боловсруулсан. Багийн гишүүд код, тест, баримтыг хянаж, шалгалтуудыг дахин ажиллуулсан.
II-4-1 баг sandbox шалгалтын скриптдээ Claude ашигласнаа тэмдэглэсэн. Дэлгэрэнгүйг тайлангийн 5.8-аас үз.

Шүүмжилсэн хувилбарын гурван commit нэг `Ulmj7` бүртгэлээс орсон тул гишүүн тус бүрийн ажлыг
Git түүхээр баталгаажуулах боломжгүй. Ажлын хуваарь тайлангийн Хавсралт А-д бий. Энэ шинэчилсэн
хувилбарын Tag/GitHub Release үүсгээгүй. Дээрх clone команд `main` салбарыг татна.

## Бэлэн кодын ашиглалт

* Linux-ийн `<linux/list.h>`-ийг **хуулаагүй**; `src/shim/linux/list.h` нь ижил нэр бүхий макроны
  өөрсдийн хэрэгжилт юм. Гуравдагч талын сан ашиглаагүй (зөвхөн C стандарт сан, POSIX, python3 stdlib).
* Сурах бичгийн snippet-д violet өнгийг `blue=43, green=226` гэж бичсэн нь struct-ийн
  талбарын дараалал (red, blue, green)-тай RGB (138, 43, 226)-г андуурсан байна. Бид зөв утгыг
  (red=138, green=43, blue=226) ашигласан.
* Нууц түлхүүр, нууц үг репозиторид байхгүй; багийн гишүүдийн нэр, оюутны код тайланд бий.

## Лиценз

Кодыг [GNU GPL хувилбар 2](LICENSE)-оор түгээнэ. Кернел модулиуд `MODULE_LICENSE("GPL")` гэж тэмдэглэгдсэн.
