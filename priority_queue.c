#include "priority_queue.h"
#include "patient.h" // De dung gioiTinhToString (neu can), hoac lay ten BN
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Cho memcpy
#include "examination.h"
// --- Ham sap xep va so sanh noi bo ---

/**
 * @brief So sanh hai LanKham de sap xep hang doi.
 * MucDoUuTien cang nho cang uu tien. Neu bang nhau, gioDangKyThanhCong cang nho cang uu tien.
 *
 * @param lk1 Con tro toi LanKham thu nhat.
 * @param lk2 Con tro toi LanKham thu hai.
 * @return Gia tri am neu lk1 uu tien hon lk2.
 * @return Gia tri duong neu lk2 uu tien hon lk1.
 * @return 0 neu uu tien tuong duong (theo logic so sanh).
 */
static int soSanhUuTienLanKham(const void* a, const void* b) {
    const LanKham* lk1 = (const LanKham*)a;
    const LanKham* lk2 = (const LanKham*)b;

    if (lk1->mucDoUuTien < lk2->mucDoUuTien) {
        return -1; // lk1 uu tien hon
    }
    if (lk1->mucDoUuTien > lk2->mucDoUuTien) {
        return 1;  // lk2 uu tien hon
    }
    // Muc do uu tien bang nhau, xet thoi gian dang ky
    if (lk1->gioDangKyThanhCong < lk2->gioDangKyThanhCong) {
        return -1; // lk1 dang ky som hon, uu tien hon
    }
    if (lk1->gioDangKyThanhCong > lk2->gioDangKyThanhCong) {
        return 1;  // lk2 dang ky som hon, uu tien hon
    }
    return 0; // Uu tien va thoi gian dang ky hoan toan giong nhau
}

/**
 * @brief Sap xep lai hang doi theo muc do uu tien va thoi gian dang ky.
 * Su dung Insertion Sort.
 *
 * @param hq Con tro toi HangDoiUuTien.
 */
static void sapXepHangDoi(HangDoiUuTien* hq) {
    if (hq == NULL || hq->soLuongHienTai <= 1) {
        return; // Khong can sap xep neu rong hoac chi co 1 phan tu
    }

    // Tu cai dat Insertion Sort
    // Khong dung qsort tu stdlib.h theo yeu cau de bai
    int i, j;
    LanKham key;
    for (i = 1; i < hq->soLuongHienTai; i++) {
        key = hq->dsChoKham[i];
        j = i - 1;

        // Di chuyen cac phan tu cua dsChoKham[0..i-1], lon hon key (kem uu tien hon key),
        // sang mot vi tri phia sau vi tri hien tai cua chung
        while (j >= 0 && soSanhUuTienLanKham(&hq->dsChoKham[j], &key) > 0) {
            hq->dsChoKham[j + 1] = hq->dsChoKham[j];
            j = j - 1;
        }
        hq->dsChoKham[j + 1] = key;
    }
}


// --- Cac ham public cua module ---

void khoiTaoHangDoi(HangDoiUuTien* hq) {
    if (hq == NULL) return;
    hq->dsChoKham = NULL;
    hq->soLuongHienTai = 0;
    hq->dungLuong = 0;
}

int themVaoHangDoi(HangDoiUuTien* hq, LanKham lanKhamMoi) {
    if (hq == NULL) return 0;

    // Chi them vao hang doi neu dang cho kham
    if (lanKhamMoi.trangThai != LK_DANG_CHO) {
        // printf("Canh bao: Chi LanKham o trang thai LK_DANG_CHO moi duoc them vao hang doi.\n");
        // Tuy vao yeu cau, co the bo qua hoac bao loi. Hien tai se bo qua.
        return 1; // Khong them nhung khong coi la loi nghiem trong
    }

    // Kiem tra xem da co trong hang doi chua (dua tren maLanKham)
    // Tranh truong hop them lai mot lan kham da co trong hang doi (neu co bug o dau do)
    for(int i=0; i < hq->soLuongHienTai; ++i) {
        if(hq->dsChoKham[i].maLanKham == lanKhamMoi.maLanKham) {
            // printf("Canh bao: Lan kham %d da co trong hang doi.\n", lanKhamMoi.maLanKham);
            return 1; // Da co, khong them lai
        }
    }


    // Tang dung luong neu can
    if (hq->soLuongHienTai >= hq->dungLuong) {
        int dungLuongMoi = (hq->dungLuong == 0) ? 10 : hq->dungLuong * 2;
        LanKham* temp = realloc(hq->dsChoKham, dungLuongMoi * sizeof(LanKham));
        if (temp == NULL) {
            perror("Loi realloc (themVaoHangDoi)");
            return 0; // That bai
        }
        hq->dsChoKham = temp;
        hq->dungLuong = dungLuongMoi;
    }

    // Them vao cuoi danh sach
    hq->dsChoKham[hq->soLuongHienTai] = lanKhamMoi;
    hq->soLuongHienTai++;

    // Sap xep lai toan bo hang doi
    sapXepHangDoi(hq);

    return 1; // Thanh cong
}

int layBenhNhanTiepTheo(HangDoiUuTien* hq, LanKham* lanKhamLayRa) {
    if (hq == NULL || laHangDoiRong(hq) || lanKhamLayRa == NULL) {
        return 0; // Hang doi rong hoac tham so khong hop le
    }

    // Phan tu dau tien luon la uu tien nhat sau khi sap xep
    *lanKhamLayRa = hq->dsChoKham[0];

    // Xoa phan tu dau tien bang cach dich chuyen cac phan tu con lai len
    for (int i = 0; i < hq->soLuongHienTai - 1; ++i) {
        hq->dsChoKham[i] = hq->dsChoKham[i + 1];
    }
    hq->soLuongHienTai--;

    // Co the thu nho mang neu can, nhung co the khong can thiet ngay lap tuc
    // de tranh realloc lien tuc.
    // if (hq->soLuongHienTai > 0 && hq->soLuongHienTai < hq->dungLuong / 4) {
    //     int dungLuongMoi = hq->dungLuong / 2;
    //     if (dungLuongMoi < 10) dungLuongMoi = 10; // Gioi han toi thieu
    //     LanKham* temp = realloc(hq->dsChoKham, dungLuongMoi * sizeof(LanKham));
    //     if (temp != NULL) {
    //         hq->dsChoKham = temp;
    //         hq->dungLuong = dungLuongMoi;
    //     }
    // } else if (hq->soLuongHienTai == 0 && hq->dsChoKham != NULL) {
    //     free(hq->dsChoKham);
    //     hq->dsChoKham = NULL;
    //     hq->dungLuong = 0;
    // }


    return 1; // Lay thanh cong
}

int laHangDoiRong(const HangDoiUuTien* hq) {
    if (hq == NULL) return 1; // Coi nhu rong neu con tro NULL
    return hq->soLuongHienTai == 0;
}

void inHangDoiHienTaiConsole(const HangDoiUuTien* hq, const BenhNhan dsBenhNhanToanBo[], int soLuongBNTB) {
    printf("\n--- HANG DOI CHO KHAM HIEN TAI ---\n");
    if (laHangDoiRong(hq)) {
        printf("Hang doi rong.\n");
        return;
    }

    // Dieu chinh do rong cot "Ma BN" va dong ke cho phu hop
    // Gia su MAX_LEN_MA_DINH_DANH_BN la 20, do rong cot co the la 18 hoac 20
    printf("-------------------------------------------------------------------------------------------------------------\n"); // Tang do rong dong ke
    printf("| %-3s | %-7s | %-18s | %-25s | %-15s | %-20s |\n", "STT", "Ma LK", "Ma BN", "Ten Benh Nhan", "Muc Do Uu Tien", "Gio Dang Ky");
    printf("-------------------------------------------------------------------------------------------------------------\n"); // Tang do rong dong ke

    for (int i = 0; i < hq->soLuongHienTai; ++i) {
        const LanKham* lk = &hq->dsChoKham[i]; // Lay mot LanKham tu hang doi
        char tenBN[101] = "N/A";               // Gia tri mac dinh neu khong tim thay ten

        // Tim ten benh nhan dua tren lk->maDinhDanh
        // Gia su dsBenhNhanToanBo la danh sach day du cac benh nhan da dang ky trong he thong
        if (lk->maDinhDanh[0] != '\0') { // Chi tim kiem neu maDinhDanh khong phai la chuoi rong
            for (int j = 0; j < soLuongBNTB; ++j) {
                // SUA LOI 1: Su dung strcmp de so sanh noi dung cua hai chuoi maDinhDanh
                if (strcmp(dsBenhNhanToanBo[j].maDinhDanh, lk->maDinhDanh) == 0) {
                    strncpy(tenBN, dsBenhNhanToanBo[j].ten, 100); // Sao chep ten benh nhan
                    tenBN[100] = '\0';                            // Dam bao ket thuc chuoi an toan
                    break; // Thoat vong lap khi da tim thay ten
                }
            }
        }


        char gioDangKyStr[20]; // Buffer cho chuoi thoi gian da dinh dang
        if (lk->gioDangKyThanhCong != 0) { // Kiem tra xem thoi gian dang ky co hop le khong
            struct tm* tm_info = localtime(&lk->gioDangKyThanhCong); // Chuyen time_t sang struct tm
            if (tm_info != NULL) { // Kiem tra xem localtime co tra ve NULL khong
                strftime(gioDangKyStr, sizeof(gioDangKyStr), "%H:%M:%S %d/%m/%Y", tm_info); // Dinh dang thoi gian
            }
            else {
                strcpy(gioDangKyStr, "Loi TGian"); // Xu ly truong hop localtime tra ve NULL
            }
        }
        else {
            strcpy(gioDangKyStr, "Chua DK"); // Truong hop thoi gian dang ky khong co (bang 0)
        }

        // SUA LOI 2: In truong lk->maDinhDanh bang dinh dang chuoi %s
        // Dong thoi, dieu chinh do rong cua cot Ma BN (vi du: %-18s)
        // Them kiem tra de in "CHUA_GAN" neu maDinhDanh trong LanKham la chuoi rong
        printf("| %-3d | %-7d | %-18s | %-25.25s | %-15s | %-20s |\n",
            i + 1,
            lk->maLanKham,
            (lk->maDinhDanh[0] != '\0' ? lk->maDinhDanh : "CHUA_GAN"), // In maDinhDanh (chuoi)
            tenBN,
            mucDoUuTienToString(lk->mucDoUuTien), // Goi ham chuyen doi Enum MucDoUuTien sang chuoi
            gioDangKyStr);
    }
    printf("-------------------------------------------------------------------------------------------------------------\n"); // Tang do rong dong ke
    printf("Tong so benh nhan dang cho: %d\n", hq->soLuongHienTai);
}

void giaiPhongHangDoi(HangDoiUuTien* hq) {
    if (hq != NULL) {
        free(hq->dsChoKham);
        hq->dsChoKham = NULL;
        hq->soLuongHienTai = 0;
        hq->dungLuong = 0;
    }
}