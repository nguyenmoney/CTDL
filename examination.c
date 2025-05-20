#include "examination.h"
#include "patient.h"        // Can cho viec tao benh nhan moi va lay ten BN
#include "csv_handler.h"    // <<<< SUA: Them include cho docDongAnToan
#include "doctor.h"         // <<<< SUA: Them include cho timBacSiTheoMa
#include "department.h"     // <<<< SUA: Them include cho timKhoaPhongTheoMa
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#ifndef TEN_BN_LK_CONSOLE_WIDTH // Su dung ten define khac de tranh xung dot neu da co TEN_BN_LK_WIDTH
#define TEN_BN_LK_CONSOLE_WIDTH 25      // Do rong hien thi Ten Benh Nhan
#define LY_DO_KHAM_LK_CONSOLE_WIDTH 20  // Do rong hien thi Ly Do Kham
#define CHAN_DOAN_LK_CONSOLE_WIDTH 15   // Do rong hien thi Chan Doan
#define GHI_CHU_BS_LK_CONSOLE_WIDTH 15  // Do rong hien thi Ghi Chu Bac Si
#define MA_BAC_SI_LK_CONSOLE_WIDTH 15   // Do rong hien thi Ma Bac Si
#endif
// Ham tien ich noi bo (static) da co trong file goc, khong can dinh nghia lai
// static void removeNewlineEx(char* str) { ... }
// static void readStringSafeEx(char* buffer, int size) { ... }
// Cac ham chuyen doi Enum <-> String van giu nguyen nhu ban da co

// --- Chuyen doi Enum <-> String --- (Giu nguyen nhu ban da co)
int my_strcasecmp(const char* s1, const char* s2) {
    int c1, c2;
    do {
        c1 = tolower((unsigned char)*s1++);
        c2 = tolower((unsigned char)*s2++);
    } while (c1 == c2 && c1 != 0); // Tiếp tục nếu ký tự giống nhau và chưa kết thúc chuỗi
    return c1 - c2;
}
const char* trangThaiLanKhamToString(TrangThaiLanKham tt) {
    switch (tt) {
        case LK_DANG_CHO: return "DangCho";
        case LK_DANG_KHAM: return "DangKham";
        case LK_DA_HOAN_THANH: return "DaHoanThanh";
        case LK_DA_HUY: return "DaHuy";
        case LK_DA_LO: return "DaLo";
        default: return "KhongXacDinh";
    }
}

TrangThaiLanKham stringToTrangThaiLanKham(const char* str) {
    if (str == NULL || strlen(str) == 0) return LK_DANG_CHO; // Them kiem tra strlen
    char lowerStr[50];
    strncpy(lowerStr, str, sizeof(lowerStr)-1); lowerStr[sizeof(lowerStr)-1] = '\0';
    for(int i = 0; lowerStr[i]; i++) lowerStr[i] = tolower(lowerStr[i]);

    if (strcmp(lowerStr, "dangcho") == 0) return LK_DANG_CHO;
    if (strcmp(lowerStr, "dangkham") == 0) return LK_DANG_KHAM;
    if (strcmp(lowerStr, "dahoanthanh") == 0) return LK_DA_HOAN_THANH;
    if (strcmp(lowerStr, "dahuy") == 0) return LK_DA_HUY;
    if (strcmp(lowerStr, "dalo") == 0) return LK_DA_LO;
    return LK_DANG_CHO; 
}

const char* mucDoUuTienToString(MucDoUuTien md) {
    switch (md) {
        case CAP_CUU: return "CapCuu";
        case KHAN_CAP: return "KhanCap";
        case THONG_THUONG: return "ThongThuong";
        case TAI_KHAM: return "TaiKham";
        case UU_TIEN_KHAC: return "UuTienKhac";
        default: return "KhongXacDinh";
    }
}

MucDoUuTien stringToMucDoUuTien(const char* str) {
    if (str == NULL || strlen(str) == 0) return THONG_THUONG; // Them kiem tra strlen
    char lowerStr[50];
    strncpy(lowerStr, str, sizeof(lowerStr)-1); lowerStr[sizeof(lowerStr)-1] = '\0';
    for(int i = 0; lowerStr[i]; i++) lowerStr[i] = tolower(lowerStr[i]);

    if (strcmp(lowerStr, "capcuu") == 0) return CAP_CUU;
    if (strcmp(lowerStr, "khancap") == 0) return KHAN_CAP;
    if (strcmp(lowerStr, "thongthuong") == 0) return THONG_THUONG;
    if (strcmp(lowerStr, "taikham") == 0) return TAI_KHAM;
    if (strcmp(lowerStr, "uutienkhac") == 0) return UU_TIEN_KHAC;
    return THONG_THUONG; 
}


// examination.c
#include "examination.h"
#include "patient.h"        // Can cho viec tao benh nhan moi va lay ten BN
#include "csv_handler.h"    // Cho xoaKyTuXuongDong
#include "doctor.h"         // Neu can timBacSiTheoMa
#include "department.h"     // Neu can timKhoaPhongTheoMa
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>          // Cho isdigit, tolower

// --- Cac ham chuyen doi Enum <-> String ---
// (Dam bao cac ham nay da duoc dinh nghia chinh xac trong examination.c hoac .h)
// const char* trangThaiLanKhamToString(TrangThaiLanKham tt) { ... }
// TrangThaiLanKham stringToTrangThaiLanKham(const char* str) { ... }
// const char* mucDoUuTienToString(MucDoUuTien md) { ... }
// MucDoUuTien stringToMucDoUuTien(const char* str) { ... }


// examination.c
// ... (cac include va ham chuyen doi Enum) ...

LanKham* docDanhSachLanKham(const char* tenFile, int* soLuong) {
    FILE* f = fopen(tenFile, "r");
    if (f == NULL) {
        fprintf(stderr, "LOI: Khong the mo file %s de doc danh sach lan kham.\n", tenFile);
        *soLuong = 0;
        return NULL;
    }

    LanKham* ds = NULL;
    int capacity = 0;
    *soLuong = 0;
    char line[2048];
    int lineNumber = 0;
    int skippedHeader = 0;

    while (fgets(line, sizeof(line), f) != NULL) {
        lineNumber++;
        char originalLineForLogging[2048];
        strncpy(originalLineForLogging, line, sizeof(originalLineForLogging) - 1);
        originalLineForLogging[sizeof(originalLineForLogging) - 1] = '\0';
        xoaKyTuXuongDong(originalLineForLogging);

        xoaKyTuXuongDong(line);
        if (strlen(line) < 5) {
            if (strlen(line) > 0 && lineNumber > 1) { // Chi canh bao neu khong phai dong dau co the la tieu de
                fprintf(stderr, "CANH BAO (docDanhSachLanKham): Dong %d qua ngan, bo qua: \"%s\"\n", lineNumber, originalLineForLogging);
            }
            continue;
        }

        if (!skippedHeader && lineNumber == 1) {
            char* tempLineForHeaderCheck = strdup(line); // Tao ban sao de strtok khong anh huong line goc
            if (tempLineForHeaderCheck) {
                char* check = strtok(tempLineForHeaderCheck, ",");
                if (check && (strcmp(check, "maLanKham") == 0 || (strlen(check) > 0 && !strspn(check, "0123456789") == strlen(check)))) {
                    fprintf(stdout, "Thong bao (docDanhSachLanKham): Bo qua dong duoc cho la tieu de: \"%s\"\n", originalLineForLogging);
                    free(tempLineForHeaderCheck);
                    skippedHeader = 1;
                    continue;
                }
                free(tempLineForHeaderCheck);
            }
        }

        if (*soLuong >= capacity) {
            capacity = (capacity == 0) ? 20 : capacity * 2;
            LanKham* temp = realloc(ds, capacity * sizeof(LanKham));
            if (temp == NULL) {
                perror("Loi cap phat bo nho (realloc docDanhSachLanKham)");
                free(ds); fclose(f); *soLuong = 0; return NULL;
            }
            ds = temp;
        }

        LanKham lk;
        // Khoi tao tat ca cac truong ve gia tri mac dinh an toan
        lk.maLanKham = 0;                lk.maDinhDanh[0] = '\0';        lk.tenBenhNhan[0] = '\0';
        lk.maBacSi[0] = '\0';            lk.maKhoaPhong[0] = '\0';       lk.ngayGioKham = 0;
        lk.lyDoKham[0] = '\0';           lk.chanDoan[0] = '\0';          lk.phuongPhapDieuTri[0] = '\0';
        lk.ghiChuBacSi[0] = '\0';        lk.ngayTaiKham.ngay = -1;       lk.ngayTaiKham.thang = -1;
        lk.ngayTaiKham.nam = -1;         lk.coLichTaiKham = 0;           lk.mucDoUuTien = THONG_THUONG;
        lk.gioDangKyThanhCong = 0;       lk.coDonThuoc = 0;              lk.trangThai = LK_DANG_CHO;
        lk.maDonThuocChiTiet = -1;

        char mucDoUuTienStr[50] = "";    char trangThaiStr[50] = "";
        long tempTimestamp;              char* token;

        // Su dung mot con tro tam de strtok khong thay doi line goc khi log loi
        char* lineCopyForStrtok = strdup(line);
        if (!lineCopyForStrtok) {
            perror("Loi strdup lineCopyForStrtok");
            continue; // Bo qua dong nay
        }
        char* p = lineCopyForStrtok; // Con tro chay cho strtok_r (neu dung) hoac cho strtok

        // 1. maLanKham
        token = strtok(p, ",");
        if (token && strlen(token) > 0 && strspn(token, "0123456789") == strlen(token)) { lk.maLanKham = atoi(token); }
        else { fprintf(stderr, "LOI DOC CSV (LanKham dong %d): MaLanKham khong hop le ('%s'). Bo qua dong: \"%s\"\n", lineNumber, token ? token : "NULL_TOKEN", originalLineForLogging); free(lineCopyForStrtok); continue; }

        // 2. maDinhDanh
        token = strtok(NULL, ",");
        if (token && strlen(token) > 0) { strncpy(lk.maDinhDanh, token, sizeof(lk.maDinhDanh) - 1); lk.maDinhDanh[sizeof(lk.maDinhDanh) - 1] = '\0'; }
        else { fprintf(stderr, "LOI DOC CSV (LanKham dong %d): Thieu maDinhDanh cho MaLK %d. Bo qua dong: \"%s\"\n", lineNumber, lk.maLanKham, originalLineForLogging); free(lineCopyForStrtok); continue; }

        // 3. tenBenhNhan
        token = strtok(NULL, ","); if (token) { strncpy(lk.tenBenhNhan, token, sizeof(lk.tenBenhNhan) - 1); lk.tenBenhNhan[sizeof(lk.tenBenhNhan) - 1] = '\0'; }

        // 4. maBacSi
        token = strtok(NULL, ","); if (token) { strncpy(lk.maBacSi, token, sizeof(lk.maBacSi) - 1); lk.maBacSi[sizeof(lk.maBacSi) - 1] = '\0'; }

        // 5. maKhoaPhong
        token = strtok(NULL, ","); if (token) { strncpy(lk.maKhoaPhong, token, sizeof(lk.maKhoaPhong) - 1); lk.maKhoaPhong[sizeof(lk.maKhoaPhong) - 1] = '\0'; }

        // 6. ngayGioKham
        token = strtok(NULL, ",");
        if (token && strlen(token) > 0 && strspn(token, "0123456789") == strlen(token)) {
            tempTimestamp = atol(token);
            if (tempTimestamp >= 0) lk.ngayGioKham = (time_t)tempTimestamp;
        }

        // 7. lyDoKham
        token = strtok(NULL, ","); if (token) { strncpy(lk.lyDoKham, token, sizeof(lk.lyDoKham) - 1); lk.lyDoKham[sizeof(lk.lyDoKham) - 1] = '\0'; }

        // 8. chanDoan
        token = strtok(NULL, ","); if (token) { strncpy(lk.chanDoan, token, sizeof(lk.chanDoan) - 1); lk.chanDoan[sizeof(lk.chanDoan) - 1] = '\0'; }

        // 9. phuongPhapDieuTri
        token = strtok(NULL, ","); if (token) { strncpy(lk.phuongPhapDieuTri, token, sizeof(lk.phuongPhapDieuTri) - 1); lk.phuongPhapDieuTri[sizeof(lk.phuongPhapDieuTri) - 1] = '\0'; }

        // 10. ghiChuBacSi
        token = strtok(NULL, ",");
        if (token) {
            if (strcmp(token, "-1") == 0) { // NEU DOC DUOC CHUOI "-1" TU CSV
                strcpy(lk.ghiChuBacSi, "N/A");   // LUU VAO BO NHO LA "N/A"
            }
            else {
                strncpy(lk.ghiChuBacSi, token, sizeof(lk.ghiChuBacSi) - 1);
                lk.ghiChuBacSi[sizeof(lk.ghiChuBacSi) - 1] = '\0';
            }
        }
        else {
            // Neu cot trong CSV hoan toan trong (,,), token co the la NULL.
            // Trong truong hop nay, coi nhu khong co ghi chu, gan la "N/A" trong bo nho.
            strcpy(lk.ghiChuBacSi, "N/A");
        }


        // 11-14. Ngay tai kham, co lich tai kham
        token = strtok(NULL, ","); if (token && strlen(token) > 0 && strspn(token, "-0123456789") == strlen(token)) lk.ngayTaiKham.ngay = atoi(token); else lk.ngayTaiKham.ngay = -1;
        token = strtok(NULL, ","); if (token && strlen(token) > 0 && strspn(token, "-0123456789") == strlen(token)) lk.ngayTaiKham.thang = atoi(token); else lk.ngayTaiKham.thang = -1;
        token = strtok(NULL, ","); if (token && strlen(token) > 0 && strspn(token, "-0123456789") == strlen(token)) lk.ngayTaiKham.nam = atoi(token); else lk.ngayTaiKham.nam = -1;
        token = strtok(NULL, ","); if (token && strlen(token) > 0 && strspn(token, "01") == strlen(token)) lk.coLichTaiKham = atoi(token); else lk.coLichTaiKham = 0;

        // 15. mucDoUuTien
        token = strtok(NULL, ",");
        if (token && strlen(token) > 0) {
            strncpy(mucDoUuTienStr, token, sizeof(mucDoUuTienStr) - 1);
            mucDoUuTienStr[sizeof(mucDoUuTienStr) - 1] = '\0';
            lk.mucDoUuTien = stringToMucDoUuTien(mucDoUuTienStr);
            // Kiem tra neu stringToMucDoUuTien tra ve mac dinh do khong khop
            if (lk.mucDoUuTien == THONG_THUONG && my_strcasecmp(mucDoUuTienStr, "ThongThuong") != 0 && strlen(mucDoUuTienStr) > 0) {
                fprintf(stderr, "CANH BAO (docDanhSachLanKham dong %d): mucDoUuTien ('%s') khong hop le cho MaLK %d. Dat ve ThongThuong.\n", lineNumber, mucDoUuTienStr, lk.maLanKham);
            }
        } // Neu khong, giu gia tri khoi tao THONG_THUONG

        // 16. gioDangKyThanhCong
        token = strtok(NULL, ",");
        if (token && strlen(token) > 0 && strspn(token, "0123456789") == strlen(token)) {
            tempTimestamp = atol(token);
            if (tempTimestamp >= 0) {
                lk.gioDangKyThanhCong = (time_t)tempTimestamp;
            }
            else {
                lk.gioDangKyThanhCong = 0;
                fprintf(stderr, "CANH BAO (docDanhSachLanKham dong %d): gioDangKyThanhCong trong CSV la so am ('%s') cho MaLK %d. Dat ve 0.\n", lineNumber, token, lk.maLanKham);
            }
        } // Neu khong, giu gia tri khoi tao 0

        // 17. coDonThuoc
        token = strtok(NULL, ","); if (token && strlen(token) > 0 && strspn(token, "01") == strlen(token)) lk.coDonThuoc = atoi(token); else lk.coDonThuoc = 0;

        // 18. trangThai
        token = strtok(NULL, ",");
        if (token && strlen(token) > 0) {
            strncpy(trangThaiStr, token, sizeof(trangThaiStr) - 1);
            trangThaiStr[sizeof(trangThaiStr) - 1] = '\0';
            lk.trangThai = stringToTrangThaiLanKham(trangThaiStr);
            // Kiem tra neu stringToTrangThaiLanKham tra ve mac dinh do khong khop
            if (lk.trangThai == LK_DANG_CHO && my_strcasecmp(trangThaiStr, "DangCho") != 0 && strlen(trangThaiStr) > 0) {
                fprintf(stderr, "CANH BAO (docDanhSachLanKham dong %d): trangThai ('%s') khong hop le cho MaLK %d. Dat ve DangCho.\n", lineNumber, trangThaiStr, lk.maLanKham);
            }
        }

        // 19. maDonThuocChiTiet
        token = strtok(NULL, "\n");
        if (token && strlen(token) > 0 && strspn(token, "-0123456789") == strlen(token)) lk.maDonThuocChiTiet = atoi(token); else lk.maDonThuocChiTiet = -1;

        free(lineCopyForStrtok); // Giai phong ban sao cua dong
        ds[*soLuong] = lk;
        (*soLuong)++;
    }
    fclose(f);
    return ds;
}




void ghiDanhSachLanKhamRaFile(const char* tenFile, const LanKham dsLanKham[], int soLuong) {
    FILE* f = fopen(tenFile, "w");
    if (f == NULL) {
        fprintf(stderr, "LOI NGHIEM TRONG: Khong the mo file %s de ghi danh sach lan kham. Cac thay doi co the khong duoc luu.\n", tenFile);
        perror("Chi tiet loi fopen"); // In ra chi tiết lỗi hệ thống (ví dụ: Permission denied)
        return;
    }

    fprintf(f, "maLanKham,maDinhDanh,tenBenhNhan,maBacSi,maKhoaPhong,ngayGioKham,lyDoKham,chanDoan,phuongPhapDieuTri,ghiChuBacSi,ngayTaiKham.ngay,ngayTaiKham.thang,ngayTaiKham.nam,coLichTaiKham,mucDoUuTien,gioDangKyThanhCong,coDonThuoc,trangThai,maDonThuoc\n");

    for (int i = 0; i < soLuong; ++i) {
        // ... (phần fprintf dữ liệu giữ nguyên) ...
        long ngayGioKhamOut = (dsLanKham[i].ngayGioKham >= 0) ? (long)dsLanKham[i].ngayGioKham : 0; //
        long gioDangKyOut = (dsLanKham[i].gioDangKyThanhCong >= 0) ? (long)dsLanKham[i].gioDangKyThanhCong : 0; //

        fprintf(f, "%d,%s,%s,%s,%s,%ld,%s,%s,%s,%s,%d,%d,%d,%d,%s,%ld,%d,%s,%d\n", //
            dsLanKham[i].maLanKham, //
            dsLanKham[i].maDinhDanh, //
            dsLanKham[i].tenBenhNhan, //
            (strlen(dsLanKham[i].maBacSi) == 0 ? "" : dsLanKham[i].maBacSi), //
            (strlen(dsLanKham[i].maKhoaPhong) == 0 ? "" : dsLanKham[i].maKhoaPhong), //
            ngayGioKhamOut, //
            dsLanKham[i].lyDoKham, //
            dsLanKham[i].chanDoan, //
            dsLanKham[i].phuongPhapDieuTri, //
            dsLanKham[i].ghiChuBacSi, //
            dsLanKham[i].ngayTaiKham.ngay, //
            dsLanKham[i].ngayTaiKham.thang, //
            dsLanKham[i].ngayTaiKham.nam, //
            dsLanKham[i].coLichTaiKham, //
            mucDoUuTienToString(dsLanKham[i].mucDoUuTien), //
            gioDangKyOut, //
            dsLanKham[i].coDonThuoc, //
            trangThaiLanKhamToString(dsLanKham[i].trangThai), //
            dsLanKham[i].maDonThuocChiTiet //
        );
    }
    fclose(f);
}


int taoMaLanKhamMoi(const LanKham dsLanKham[], int soLuong) {
    if (soLuong == 0) return 1;
    int maxId = 0;
    for (int i = 0; i < soLuong; ++i) {
        if (dsLanKham[i].maLanKham > maxId) maxId = dsLanKham[i].maLanKham;
    }
    return maxId + 1;
}

int themLanKham(LanKham** dsLanKhamConTro, int* soLuongConTro, LanKham lanKhamMoi, const char* tenFile) {
    LanKham* temp = realloc(*dsLanKhamConTro, (*soLuongConTro + 1) * sizeof(LanKham));
    if (temp == NULL) {
        perror("Loi realloc (themLanKham)");
        return 0;
    }
    *dsLanKhamConTro = temp;
    (*dsLanKhamConTro)[*soLuongConTro] = lanKhamMoi;
    (*soLuongConTro)++;
    ghiDanhSachLanKhamRaFile(tenFile, *dsLanKhamConTro, *soLuongConTro);
    return 1;
}


int dangKyKhamMoi(LanKham** dsLanKhamConTro, int* soLuongLanKhamConTro,
                  BenhNhan** dsBenhNhanConTro, int* soLuongBenhNhanConTro,
                  const char* fileLanKhamCSV, const char* fileBenhNhanCSV) {
    char maBN[51];
    char bufferInput[100];

    printf("Nhap Ma Dinh Danh Benh Nhan: ");
    if (docDongAnToan(bufferInput, sizeof(bufferInput), stdin) == NULL || sscanf(bufferInput, "%50s", maBN) != 1) {
        printf("Ma dinh danh khong hop le.\n");
        return -1;
    }

    int bnTonTai = 0;
    for (int i = 0; i < *soLuongBenhNhanConTro; ++i) {
        if (strcmp((*dsBenhNhanConTro)[i].maDinhDanh, maBN) == 0) {
            bnTonTai = 1;
            break;
        }
    }

    if (!bnTonTai) {
            printf("Benh nhan voi Ma Dinh Danh %s khong ton tai.\n", maBN); // maBN la ma nguoi dung nhap ban dau
            char choice_str[10];
            printf("Ban co muon tao moi ho so benh nhan khong? (c/k): ");
            docDongAnToan(choice_str, sizeof(choice_str), stdin);
            if (strlen(choice_str) > 0 && tolower(choice_str[0]) == 'c') {
                BenhNhan bnMoi;
                printf("--- Nhap Thong Tin Benh Nhan Moi ---\n");
                // Goi ham nhapThongTinBenhNhanTuBanPhim moi
                // Ma Dinh Danh se duoc nhap va kiem tra duy nhat trong ham nay
                // Nguoi dung co the duoc goi y nhap lai maBN (ma khong tim thay) hoac nhap ma moi.
                // Ham nhapThongTinBenhNhanTuBanPhim se lo viec do.
                if (nhapThongTinBenhNhanTuBanPhim(&bnMoi, *dsBenhNhanConTro, *soLuongBenhNhanConTro)) { // <<<< THAY DOI O DAY
                    // ngayTaoHoSo da duoc gan trong nhapThongTin...
                    if (!themBenhNhan(dsBenhNhanConTro, soLuongBenhNhanConTro, bnMoi, fileBenhNhanCSV)) {
                        printf("Loi: Khong thể tao benh nhan moi.\n");
                        return -1;
                    }
                    strncpy(maBN, bnMoi.maDinhDanh, sizeof(maBN)-1); // Cap nhat maBN voi benh nhan vua tao de tiep tuc dang ky kham
                    maBN[sizeof(maBN)-1] = '\0';
                    printf("Da tao benh nhan moi voi Ma Dinh Danh: %s\n", maBN);
                } else {
                    printf("Huy tao benh nhan moi. Dang ky kham bi huy.\n");
                    return -1;
                }
            } else {
                printf("Dang ky kham bi huy do benh nhan khong ton tai va khong tao moi.\n");
                return -1;
            }
        }

    LanKham lkMoi;
    lkMoi.maLanKham = taoMaLanKhamMoi(*dsLanKhamConTro, *soLuongLanKhamConTro);

    // Gán maDinhDanh cho lkMoi
    strncpy(lkMoi.maDinhDanh, maBN, sizeof(lkMoi.maDinhDanh) - 1);
    lkMoi.maDinhDanh[sizeof(lkMoi.maDinhDanh) - 1] = '\0';

    // >>> BỔ SUNG: Tìm và gán tên bệnh nhân cho lkMoi.tenBenhNhan <<<
    const BenhNhan* bnTimThay = NULL; // Sử dụng dsBenhNhanConTro đã được cập nhật nếu có tạo mới
    for (int i = 0; i < *soLuongBenhNhanConTro; ++i) {
        if (strcmp((*dsBenhNhanConTro)[i].maDinhDanh, maBN) == 0) {
            bnTimThay = &(*dsBenhNhanConTro)[i];
            break;
        }
    }

    if (bnTimThay != NULL) {
        strncpy(lkMoi.tenBenhNhan, bnTimThay->ten, sizeof(lkMoi.tenBenhNhan) - 1);
        lkMoi.tenBenhNhan[sizeof(lkMoi.tenBenhNhan) - 1] = '\0';
    }
    else {
        // Điều này không nên xảy ra nếu logic kiểm tra/tạo bệnh nhân ở trên đã đúng
        fprintf(stderr, "CANH BAO: Khong tim thay ten cho ma benh nhan %s khi dang ky kham. Dat ten mac dinh.\n", maBN);
        strcpy(lkMoi.tenBenhNhan, "Khong Xac Dinh");
    }

    printf("Nhap Ly Do Kham: ");
    docDongAnToan(lkMoi.lyDoKham, sizeof(lkMoi.lyDoKham), stdin);

    char mucDoUTStr[30];
    printf("Nhap Muc Do Uu Tien (CapCuu, KhanCap, ThongThuong, TaiKham, UuTienKhac): ");
    docDongAnToan(mucDoUTStr, sizeof(mucDoUTStr), stdin);
    lkMoi.mucDoUuTien = stringToMucDoUuTien(mucDoUTStr);

    lkMoi.gioDangKyThanhCong = time(NULL);
    lkMoi.trangThai = LK_DANG_CHO;

    // Các giá trị khởi tạo khác
    strcpy(lkMoi.maBacSi, "CHUA_GAN");
    strcpy(lkMoi.maKhoaPhong, "CHUA_GAN");
    lkMoi.ngayGioKham = 0;
    strcpy(lkMoi.chanDoan, "CHUA_KHAM");
    strcpy(lkMoi.phuongPhapDieuTri, "CHUA_KHAM");
    strcpy(lkMoi.ghiChuBacSi, "-1"); // Để trống
    lkMoi.ngayTaiKham.ngay = -1; lkMoi.ngayTaiKham.thang = -1; lkMoi.ngayTaiKham.nam = -1;
    lkMoi.coLichTaiKham = 0;
    lkMoi.coDonThuoc = 0;
    lkMoi.maDonThuocChiTiet = -1;

    if (themLanKham(dsLanKhamConTro, soLuongLanKhamConTro, lkMoi, fileLanKhamCSV)) {
        printf("Dang ky kham thanh cong! Ma Lan Kham: %d. Ten BN: %s\n", lkMoi.maLanKham, lkMoi.tenBenhNhan);
        return lkMoi.maLanKham;
    }
    else {
        printf("Loi: Khong the dang ky kham.\n");
        return -1;
    }
}

// Ham capNhatThongTinLanKham da duoc chinh sua lai hoan chinh

int capNhatThongTinLanKham(LanKham dsLanKham[], int soLuongLanKham, int maLanKhamCanCapNhat,
    const BacSi dsBacSi[], int soLuongBacSi,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
    const char* fileLanKhamCSV) {
    int indexLK = -1;
    for (int i = 0; i < soLuongLanKham; ++i) {
        if (dsLanKham[i].maLanKham == maLanKhamCanCapNhat) {
            indexLK = i;
            break;
        }
    }

    if (indexLK == -1) {
        printf("Khong tim thay Lan Kham voi Ma: %d\n", maLanKhamCanCapNhat);
        return 0;
    }

    LanKham* lk = &dsLanKham[indexLK];

    if (lk->trangThai == LK_DA_HOAN_THANH || lk->trangThai == LK_DA_HUY || lk->trangThai == LK_DA_LO) {
        printf("Lan kham (Ma: %d) da o trang thai '%s'. Khong the cap nhat theo quy trinh nay.\n",
            lk->maLanKham, trangThaiLanKhamToString(lk->trangThai));
        return 0;
    }
    if (lk->trangThai != LK_DANG_CHO && lk->trangThai != LK_DANG_KHAM) {
        printf("Lan kham (Ma: %d) khong o trang thai phu hop de cap nhat (hien tai: '%s').\n",
            lk->maLanKham, trangThaiLanKhamToString(lk->trangThai));
        return 0;
    }

    printf("\n--- CAP NHAT THONG TIN LAN KHAM MA: %d (Benh nhan: %s) ---\n", lk->maLanKham, lk->tenBenhNhan);

    char denKhamChoice_str[10];
    printf("Benh nhan co den kham khong? (c/k): ");
    docDongAnToan(denKhamChoice_str, sizeof(denKhamChoice_str), stdin);

    if (strlen(denKhamChoice_str) > 0 && tolower(denKhamChoice_str[0]) == 'k') {
        printf("Ly do benh nhan khong den: \n");
        printf("  1. Da Huy Hen/Kham\n");
        printf("  2. Khong Co Mat (Lo Hen/Kham)\n");

        int lyDoVangMatChoice;
        char bufferLyDo[10];
        printf("Lua chon cua ban (1-2): ");
        if (docDongAnToan(bufferLyDo, sizeof(bufferLyDo), stdin) != NULL && sscanf(bufferLyDo, "%d", &lyDoVangMatChoice) == 1) {
            if (lyDoVangMatChoice == 1) {
                lk->trangThai = LK_DA_HUY;
                printf("Da cap nhat trang thai: Da Huy Hen/Kham.\n");
            }
            else if (lyDoVangMatChoice == 2) {
                lk->trangThai = LK_DA_LO;
                printf("Da cap nhat trang thai: Da Lo Hen/Kham (Khong Co Mat).\n");
            }
            else {
                printf("Lua chon ly do vang mat khong hop le. Khong thay doi.\n");
                // Khong return o day de tranh ghi file voi thong tin co the chua dung
                return 0; // Hoac khong lam gi va de nguoi dung thu lai tu menu
            }
        }
        else {
            printf("Nhap khong hop le cho ly do vang mat. Khong thay doi.\n");
            return 0;
        }

        strcpy(lk->chanDoan, (lk->trangThai == LK_DA_HUY) ? "HUY_KHAM" : "LO_HEN_KHONG_KHAM");
        strcpy(lk->phuongPhapDieuTri, "N/A");
        strcpy(lk->ghiChuBacSi, "Khong"); // **SUA DOI: Khi vang mat, ghi chu la "Khong"**
        strcpy(lk->maBacSi, "N/A_VANGMAT"); // De phan biet voi N/A thong thuong
        lk->coLichTaiKham = 0;
        lk->ngayTaiKham.ngay = -1; lk->ngayTaiKham.thang = -1; lk->ngayTaiKham.nam = -1;
        lk->coDonThuoc = 0;
        lk->maDonThuocChiTiet = -1;

    }
    else if (strlen(denKhamChoice_str) > 0 && tolower(denKhamChoice_str[0]) == 'c') {
        lk->trangThai = LK_DA_HOAN_THANH;

        char maBacSiNhap[sizeof(lk->maBacSi)];
        const BacSi* bacSiTimThay = NULL;
        char maKhoaPhongTheoBacSi[sizeof(lk->maKhoaPhong)] = "";

        // Nhap Ma Bac Si (bat buoc)
        do {
            printf("Nhap Ma Bac Si kham (khong de trong): ");
            docDongAnToan(maBacSiNhap, sizeof(maBacSiNhap), stdin);
            for (int i = 0; maBacSiNhap[i]; i++) maBacSiNhap[i] = toupper(maBacSiNhap[i]);

            if (strlen(maBacSiNhap) == 0) {
                printf("LOI: Ma Bac Si khong duoc de trong. Vui long nhap lai.\n");
                strcpy(lk->maBacSi, ""); // De vong lap tiep tuc
            }
            else {
                bacSiTimThay = timBacSiTheoMa(dsBacSi, soLuongBacSi, maBacSiNhap);
                if (bacSiTimThay != NULL) {
                    strcpy(lk->maBacSi, bacSiTimThay->maBacSi);
                    if (strlen(bacSiTimThay->maKhoaPhong) > 0) {
                        strcpy(maKhoaPhongTheoBacSi, bacSiTimThay->maKhoaPhong);
                    }
                    printf("Bac si tim thay: %s. Khoa phong mac dinh: %s\n", bacSiTimThay->tenBacSi, strlen(maKhoaPhongTheoBacSi) > 0 ? maKhoaPhongTheoBacSi : "KHONG_CO");
                    break; // Thoat vong lap khi co ma hop le
                }
                else {
                    printf("CANH BAO: Ma Bac Si '%s' khong tim thay. Ban co chac muon su dung ma nay? (c/k): ", maBacSiNhap);
                    char confirmBS[10];
                    docDongAnToan(confirmBS, sizeof(confirmBS), stdin);
                    if (tolower(confirmBS[0]) == 'c') {
                        strcpy(lk->maBacSi, maBacSiNhap); // Chap nhan ma khong co trong danh sach
                        maKhoaPhongTheoBacSi[0] = '\0'; // Khong co KP theo BS neu BS khong co trong DS
                        break; // Thoat vong lap
                    }
                    else {
                        strcpy(lk->maBacSi, ""); // Reset de nhap lai
                    }
                }
            }
        } while (strlen(lk->maBacSi) == 0);


        // Nhap Ma Khoa Phong (bat buoc)
        // Logic nay phuc tap, can dam bao no khong de maKhoaPhong bi trong neu bac si khong co khoa phong mac dinh
        // va nguoi dung cung khong nhap.
        printf("Nhap Ma Khoa Phong kham (Ma KP hien tai: %s, Ma KP theo bac si vua chon: %s).\nNhan Enter de dung Ma KP theo bac si (neu co va hop le), hoac nhap Ma KP moi: ",
            (strlen(lk->maKhoaPhong) > 0 && strcmp(lk->maKhoaPhong, "CHUA_GAN") != 0) ? lk->maKhoaPhong : "CHUA_GAN",
            strlen(maKhoaPhongTheoBacSi) > 0 ? maKhoaPhongTheoBacSi : "KHONG_CO");

        char maKPMoi[sizeof(lk->maKhoaPhong)];
        docDongAnToan(maKPMoi, sizeof(maKPMoi), stdin);

        if (strlen(maKPMoi) > 0) { // Nguoi dung NHAP ma khoa phong moi
            for (int i = 0; maKPMoi[i]; i++) maKPMoi[i] = toupper(maKPMoi[i]);
            if (timKhoaPhongTheoMa(dsKhoaPhong, soLuongKhoaPhong, maKPMoi) != NULL) {
                strcpy(lk->maKhoaPhong, maKPMoi);
                printf("Da cap nhat Ma Khoa Phong thanh: %s\n", lk->maKhoaPhong);
            }
            else {
                printf("CANH BAO: Ma Khoa Phong moi '%s' khong tim thay.\n", maKPMoi);
                if (strlen(maKhoaPhongTheoBacSi) > 0 && timKhoaPhongTheoMa(dsKhoaPhong, soLuongKhoaPhong, maKhoaPhongTheoBacSi) != NULL) {
                    strcpy(lk->maKhoaPhong, maKhoaPhongTheoBacSi);
                    printf("Da su dung Ma Khoa Phong theo bac si: %s\n", lk->maKhoaPhong);
                }
                else if (strlen(lk->maKhoaPhong) == 0 || strcmp(lk->maKhoaPhong, "CHUA_GAN") == 0) {
                    printf("LOI: Ma Khoa Phong khong hop le va khong co Ma KP theo bac si. Vui long sua lai sau.\n");
                    strcpy(lk->maKhoaPhong, "CHUA_GAN_KP_LOI"); // Can co co che nhap lai
                }
                else {
                    printf("Giu nguyen Ma Khoa Phong hien tai: %s.\n", lk->maKhoaPhong);
                }
            }
        }
        else { // Nguoi dung BO TRONG
            if (strlen(maKhoaPhongTheoBacSi) > 0 && timKhoaPhongTheoMa(dsKhoaPhong, soLuongKhoaPhong, maKhoaPhongTheoBacSi) != NULL) {
                strcpy(lk->maKhoaPhong, maKhoaPhongTheoBacSi);
                printf("Da su dung Ma Khoa Phong theo bac si: %s\n", lk->maKhoaPhong);
            }
            else if (strlen(lk->maKhoaPhong) == 0 || strcmp(lk->maKhoaPhong, "CHUA_GAN") == 0) {
                printf("LOI: Ma Khoa Phong la bat buoc va khong co Ma KP hop le theo bac si. Vui long sua lai sau.\n");
                strcpy(lk->maKhoaPhong, "CHUA_GAN_KP_LOI"); // Can co co che nhap lai
            }
            else {
                printf("Khong co Ma KP theo bac si hop le. Giu nguyen Ma Khoa Phong hien tai: %s\n", lk->maKhoaPhong);
            }
        }
        // Kiem tra cuoi cung cho maKhoaPhong
        if (strlen(lk->maKhoaPhong) == 0 || strcmp(lk->maKhoaPhong, "CHUA_GAN") == 0 || strcmp(lk->maKhoaPhong, "CHUA_GAN_KP_LOI") == 0) {
            printf("CANH BAO: Ma Khoa Phong chua hop le. Hay cap nhat sau tai chuc nang Sua Chi Tiet Lan Kham.\n");
            // Khong return, de nguoi dung nhap cac thong tin khac
        }


        printf("Nhap Chan Doan (khong de trong): ");
        docDongAnToan(lk->chanDoan, sizeof(lk->chanDoan), stdin);
        if (strlen(lk->chanDoan) == 0) {
            strcpy(lk->chanDoan, "CHUA_NHAP_CD");
            printf("Chan Doan duoc dat mac dinh la 'CHUA_NHAP_CD' do bo trong.\n");
        }

        printf("Nhap Phuong Phap Dieu Tri (khong de trong): ");
        docDongAnToan(lk->phuongPhapDieuTri, sizeof(lk->phuongPhapDieuTri), stdin);
        if (strlen(lk->phuongPhapDieuTri) == 0) {
            strcpy(lk->phuongPhapDieuTri, "CHUA_NHAP_PPTT");
            printf("Phuong Phap Dieu Tri duoc dat mac dinh la 'CHUA_NHAP_PPTT' do bo trong.\n");
        }

        printf("Nhap Ghi Chu Bac Si (de trong hoac nhap 'Khong'/'N/A' se luu la 'Khong'): ");
        docDongAnToan(lk->ghiChuBacSi, sizeof(lk->ghiChuBacSi), stdin);
        char ghiChuLowerCapNhat[sizeof(lk->ghiChuBacSi)];
        strncpy(ghiChuLowerCapNhat, lk->ghiChuBacSi, sizeof(ghiChuLowerCapNhat) - 1);
        ghiChuLowerCapNhat[sizeof(ghiChuLowerCapNhat) - 1] = '\0';
        for (int k = 0; ghiChuLowerCapNhat[k]; ++k) ghiChuLowerCapNhat[k] = tolower(ghiChuLowerCapNhat[k]);

        if (strlen(lk->ghiChuBacSi) == 0 || strcmp(ghiChuLowerCapNhat, "khong") == 0 || strcmp(ghiChuLowerCapNhat, "n/a") == 0) {
            strcpy(lk->ghiChuBacSi, "Khong"); // **SUA DOI: Chuan hoa thanh "Khong" (viet hoa K)**
        }
        // Neu nguoi dung nhap ghi chu khac, se giu nguyen

        char taiKhamChoice_str_capnhat[10]; // Đổi tên biến để tránh trùng lặp nếu có
        printf("Co lich tai kham khong? (c/k): ");
        docDongAnToan(taiKhamChoice_str_capnhat, sizeof(taiKhamChoice_str_capnhat), stdin);
        if (strlen(taiKhamChoice_str_capnhat) > 0 && tolower(taiKhamChoice_str_capnhat[0]) == 'c') {
            lk->coLichTaiKham = 1;
            printf("Nhap Ngay Tai Kham (ngay thang nam, cach nhau boi khoang trang): ");
            char bufferNgayTK[30];
            if (docDongAnToan(bufferNgayTK, sizeof(bufferNgayTK), stdin) != NULL &&
                sscanf(bufferNgayTK, "%d %d %d", &lk->ngayTaiKham.ngay, &lk->ngayTaiKham.thang, &lk->ngayTaiKham.nam) == 3) {
                // Kiem tra tinh hop le (tuy chon them)
            }
            else {
                printf("Ngay tai kham nhap khong hop le. Dat mac dinh khong co lich tai kham.\n");
                lk->coLichTaiKham = 0;
                lk->ngayTaiKham.ngay = -1; lk->ngayTaiKham.thang = -1; lk->ngayTaiKham.nam = -1;
            }
        }
        else {
            lk->coLichTaiKham = 0;
            lk->ngayTaiKham.ngay = -1; lk->ngayTaiKham.thang = -1; lk->ngayTaiKham.nam = -1;
        }

        char donThuocChoice_str_capnhat[10]; // Đổi tên biến
        printf("Co ke don thuoc khong? (c/k): ");
        docDongAnToan(donThuocChoice_str_capnhat, sizeof(donThuocChoice_str_capnhat), stdin);
        if (strlen(donThuocChoice_str_capnhat) > 0 && tolower(donThuocChoice_str_capnhat[0]) == 'c') {
            lk->coDonThuoc = 1;
            lk->maDonThuocChiTiet = lk->maLanKham;
            printf("Da ghi nhan co don thuoc (Ma Don Thuoc: %d). Chi tiet don thuoc se duoc cap nhat o Module Don Thuoc.\n", lk->maDonThuocChiTiet);
        }
        else {
            lk->coDonThuoc = 0;
            lk->maDonThuocChiTiet = -1;
        }
        printf("Da cap nhat thong tin lan kham thanh cong (trang thai: Da Hoan Thanh).\n");
    }
    else {
        printf("Lua chon khong hop le cho cau hoi benh nhan co den kham. Khong thay doi.\n");
        return 0; // Khong ghi file neu lua chon khong hop le
    }

    ghiDanhSachLanKhamRaFile(fileLanKhamCSV, dsLanKham, soLuongLanKham);
    return 1;
}


int xoaLanKham(LanKham** dsLanKhamConTro, int* soLuongConTro, int maLanKhamCanXoa, const char* tenFile) {
    int viTri = -1;
    for (int i = 0; i < *soLuongConTro; ++i) {
        if ((*dsLanKhamConTro)[i].maLanKham == maLanKhamCanXoa) {
            viTri = i;
            break;
        }
    }
    if (viTri != -1) {
        for (int i = viTri; i < (*soLuongConTro - 1); ++i) {
            (*dsLanKhamConTro)[i] = (*dsLanKhamConTro)[i + 1];
        }
        (*soLuongConTro)--;
        if (*soLuongConTro == 0) {
            free(*dsLanKhamConTro); *dsLanKhamConTro = NULL;
        } else {
            LanKham* temp = realloc(*dsLanKhamConTro, (*soLuongConTro) * sizeof(LanKham));
             if (temp == NULL && *soLuongConTro > 0) { /* Xu ly loi realloc neu can */ }
             else if (*soLuongConTro > 0) *dsLanKhamConTro = temp;
             else { free(*dsLanKhamConTro); *dsLanKhamConTro = NULL;} 
        }
        ghiDanhSachLanKhamRaFile(tenFile, *dsLanKhamConTro, *soLuongConTro);
        return 1;
    }
    return 0;
}

// examination.c
#include "examination.h"
#include "patient.h"
#include "csv_handler.h" // Cho rutGonChuoiHienThi
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

// Dinh nghia do rong cot cho viec rut gon (dat o day hoac trong examination.h neu dung o nhieu noi)
#ifndef TEN_BN_LK_CONSOLE_WIDTH // Su dung ten define khac de tranh xung dot neu da co TEN_BN_LK_WIDTH
#define TEN_BN_LK_CONSOLE_WIDTH 25      // Do rong hien thi Ten Benh Nhan
#define LY_DO_KHAM_LK_CONSOLE_WIDTH 20  // Do rong hien thi Ly Do Kham
#define CHAN_DOAN_LK_CONSOLE_WIDTH 15   // Do rong hien thi Chan Doan
#define GHI_CHU_BS_LK_CONSOLE_WIDTH 15  // Do rong hien thi Ghi Chu Bac Si
#define MA_BAC_SI_LK_CONSOLE_WIDTH 15   // Do rong hien thi Ma Bac Si
#endif


// ... (cac ham khac nhu trangThaiLanKhamToString, stringToMucDoUuTien, v.v...) ...

void inDanhSachLanKhamConsole(const LanKham dsLanKham[], int soLuongLanKham,
    const BenhNhan dsBenhNhan[], int soLuongBenhNhan) {
    printf("\n--- DANH SACH LAN KHAM BENH ---\n");
    if (soLuongLanKham == 0) {
        printf("Chua co lan kham nao.\n");
        return;
    }

    // Tieu de chung cho tat ca cac cot
    // Can tinh toan lai tong do rong dong ke cho phu hop
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    // Chinh sua lai tieu de cho phu hop voi cac cot se hien thi
    printf("| %-5s | %-7s | %-18s | %-*s | %-12s | %-15s | %-*s | %-*s | %-10s | %-10s | %-10s | %-*s | %-*s |\n",
        "STT", "Ma LK", "Ma BN",
        TEN_BN_LK_CONSOLE_WIDTH, "Ten Benh Nhan",
        "Trang Thai", "Muc Do UT",
        LY_DO_KHAM_LK_CONSOLE_WIDTH, "Ly Do Kham", // Ly do kham se luon hien thi
        CHAN_DOAN_LK_CONSOLE_WIDTH, "Chan Doan",   // Cac cot sau day se co dieu kien
        "Tai Kham", "Don Thuoc", "Ma DT",
        MA_BAC_SI_LK_CONSOLE_WIDTH, "Ma Bac Si",
        GHI_CHU_BS_LK_CONSOLE_WIDTH, "Ghi Chu BS");
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < soLuongLanKham; ++i) {
        const LanKham* lk = &dsLanKham[i];
        char tenBNHienThi[101] = "N/A"; // Buffer de lay ten BN goc

        // Logic lay ten benh nhan
        if (strlen(lk->tenBenhNhan) > 0 && strcmp(lk->tenBenhNhan, "Khong Biet") != 0 && strcmp(lk->tenBenhNhan, "Khong Xac Dinh") != 0) {
            strncpy(tenBNHienThi, lk->tenBenhNhan, 100);
        }
        else {
            for (int j = 0; j < soLuongBenhNhan; ++j) {
                if (strcmp(dsBenhNhan[j].maDinhDanh, lk->maDinhDanh) == 0) {
                    strncpy(tenBNHienThi, dsBenhNhan[j].ten, 100);
                    break;
                }
            }
        }
        tenBNHienThi[100] = '\0'; // Dam bao null-terminated

        // Rut gon cac chuoi se hien thi
        char tenBNRutGon[TEN_BN_LK_CONSOLE_WIDTH + 1];
        rutGonChuoiHienThi(tenBNRutGon, sizeof(tenBNRutGon), tenBNHienThi, TEN_BN_LK_CONSOLE_WIDTH);

        char lyDoKhamRutGon[LY_DO_KHAM_LK_CONSOLE_WIDTH + 1];
        rutGonChuoiHienThi(lyDoKhamRutGon, sizeof(lyDoKhamRutGon), lk->lyDoKham, LY_DO_KHAM_LK_CONSOLE_WIDTH);

        // In cac thong tin co ban luon hien thi cho moi lan kham
        printf("| %-5d | %-7d | %-18s | %-*s | %-12s | %-15s | %-*s |", // Ket thuc sau Ly Do Kham
            i + 1,
            lk->maLanKham,
            lk->maDinhDanh,
            TEN_BN_LK_CONSOLE_WIDTH, tenBNRutGon,
            trangThaiLanKhamToString(lk->trangThai),
            mucDoUuTienToString(lk->mucDoUuTien),
            LY_DO_KHAM_LK_CONSOLE_WIDTH, lyDoKhamRutGon);

        // Kiem tra trang thai de quyet dinh hien thi cac thong tin con lai
        if (lk->trangThai == LK_DANG_CHO || lk->trangThai == LK_DANG_KHAM) {
            // Neu la DangCho hoac DangKham: Cac truong sau se la "N/A" hoac "-"
            printf(" %-*s | %-10s | %-10s | %-10s | %-*s | %-*s |\n",
                CHAN_DOAN_LK_CONSOLE_WIDTH, "N/A",      // Chan Doan
                "-",                                   // Tai Kham
                "-",                                   // Don Thuoc
                "-",                                   // Ma DT
                MA_BAC_SI_LK_CONSOLE_WIDTH, "N/A",     // Ma Bac Si
                GHI_CHU_BS_LK_CONSOLE_WIDTH, "N/A"      // Ghi Chu BS
            );
        }
        else if (lk->trangThai == LK_DA_HUY || lk->trangThai == LK_DA_LO) {
            // Neu la DaLo hoac DaHuy: Cac truong sau se la "N/A" hoac "-"
            printf(" %-*s | %-10s | %-10s | %-10s | %-*s | %-*s |\n",
                CHAN_DOAN_LK_CONSOLE_WIDTH, "N/A",      // Chan Doan
                "-",                                   // Tai Kham
                "-",                                   // Don Thuoc
                "-",                                   // Ma DT
                MA_BAC_SI_LK_CONSOLE_WIDTH, "N/A",     // Ma Bac Si
                GHI_CHU_BS_LK_CONSOLE_WIDTH, "N/A"      // Ghi Chu BS
            );
        }
        else {
            // Neu la DaHoanThanh, DaHuy, DaLo: Hien thi thong tin day du (da qua rut gon)
            char chanDoanRutGon[CHAN_DOAN_LK_CONSOLE_WIDTH + 1];
            rutGonChuoiHienThi(chanDoanRutGon, sizeof(chanDoanRutGon), lk->chanDoan, CHAN_DOAN_LK_CONSOLE_WIDTH);

            char ngayTaiKhamStr[12] = "-";
            if (lk->coLichTaiKham &&
                lk->ngayTaiKham.ngay >= 1 && lk->ngayTaiKham.ngay <= 31 &&
                lk->ngayTaiKham.thang >= 1 && lk->ngayTaiKham.thang <= 12 &&
                lk->ngayTaiKham.nam >= 1900 && lk->ngayTaiKham.nam <= 9999) { // Kiem tra ngay hop le
                snprintf(ngayTaiKhamStr, sizeof(ngayTaiKhamStr), "%02d/%02d/%04d", lk->ngayTaiKham.ngay, lk->ngayTaiKham.thang, lk->ngayTaiKham.nam);
            }

            char maBacSiDisplay[MA_BAC_SI_LK_CONSOLE_WIDTH + 1];
            const char* maBacSiHienThi;
            if (strlen(lk->maBacSi) == 0 ||
                strcmp(lk->maBacSi, "CHUA_GAN") == 0 ||
                strcmp(lk->maBacSi, "N/A") == 0) {
                maBacSiHienThi = "N/A";
            }
            else {
                maBacSiHienThi = lk->maBacSi;
            }
            rutGonChuoiHienThi(maBacSiDisplay, sizeof(maBacSiDisplay), maBacSiHienThi, MA_BAC_SI_LK_CONSOLE_WIDTH);

            // Khai bao va chuan bi ghiChuBSRutGon (PHAN BAN DA CHI RA)
            char ghiChuBSRutGon[GHI_CHU_BS_LK_CONSOLE_WIDTH + 1];
            const char* ghiChuHienThiChoBang;
            // Logic: docDanhSachLanKham da chuyen "-1" hoac "" tu CSV thanh "N/A" trong bo nho (lk->ghiChuBacSi)
            if (strcmp(lk->ghiChuBacSi, "N/A") == 0 || strlen(lk->ghiChuBacSi) == 0) { // Kiem tra them strlen cho chac
                ghiChuHienThiChoBang = "N/A"; // Hoac ban muon la "-"
            }
            else {
                ghiChuHienThiChoBang = lk->ghiChuBacSi;
            }
            rutGonChuoiHienThi(ghiChuBSRutGon, sizeof(ghiChuBSRutGon), ghiChuHienThiChoBang, GHI_CHU_BS_LK_CONSOLE_WIDTH);

            printf(" %-*s | %-10s | %-10s | %-10d | %-*s | %-*s |\n",
                CHAN_DOAN_LK_CONSOLE_WIDTH, chanDoanRutGon,
                ngayTaiKhamStr,
                lk->coDonThuoc ? "Co" : "Khong",
                lk->maDonThuocChiTiet,
                MA_BAC_SI_LK_CONSOLE_WIDTH, maBacSiDisplay,
                GHI_CHU_BS_LK_CONSOLE_WIDTH, ghiChuBSRutGon
            );
        }
    }
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

void timKiemLanKham(const LanKham dsLanKham[], int soLuongLanKham,
    const char* tieuChi, const char* giaTriTimKiem,
    LanKham** ketQuaTimKiem, int* soLuongKetQua) {
    *ketQuaTimKiem = NULL;
    *soLuongKetQua = 0;
    if (soLuongLanKham == 0 || giaTriTimKiem == NULL || tieuChi == NULL || strlen(giaTriTimKiem) == 0) {
        // Khong tim kiem neu khong co du lieu hoac tieu chi/gia tri tim kiem khong hop le
        return;
    }

    LanKham* dsKQTemp = (LanKham*)malloc(soLuongLanKham * sizeof(LanKham)); // Cap phat bo nho cho ket qua tam thoi
    if (dsKQTemp == NULL) {
        perror("Loi malloc trong timKiemLanKham (dsKQTemp)");
        return; // Khong the cap phat bo nho
    }
    int count = 0; // So luong ket qua tim thay

    char tieuChiLower[50];
    strncpy(tieuChiLower, tieuChi, sizeof(tieuChiLower) - 1);
    tieuChiLower[sizeof(tieuChiLower) - 1] = '\0';
    for (int i = 0; tieuChiLower[i]; i++) tieuChiLower[i] = tolower(tieuChiLower[i]); // Chuyen tieu chi sang chu thuong

    char giaTriTimKiemLower[100]; // Buffer cho gia tri tim kiem da chuyen sang chu thuong
    // Chi chuyen giaTriTimKiem sang chu thuong neu tieu chi la tim theo chuoi (vi du: trangthai, bacsi, madinhdanhbn)
    // Neu tim theo maLanKham (so), khong can chuyen
    if (strcmp(tieuChiLower, "trangthai") == 0 ||
        strcmp(tieuChiLower, "bacsi") == 0 ||
        strcmp(tieuChiLower, "madinhdanhbn") == 0) { // MaDinhDanhBN cung nen so sanh khong phan biet hoa thuong neu can
        strncpy(giaTriTimKiemLower, giaTriTimKiem, sizeof(giaTriTimKiemLower) - 1);
        giaTriTimKiemLower[sizeof(giaTriTimKiemLower) - 1] = '\0';
        for (int i = 0; giaTriTimKiemLower[i]; i++) giaTriTimKiemLower[i] = tolower(giaTriTimKiemLower[i]);
    }
    else {
        // Neu tieu chi khac (vi du: malankham la so), giu nguyen giaTriTimKiem goc
        strncpy(giaTriTimKiemLower, giaTriTimKiem, sizeof(giaTriTimKiemLower) - 1);
        giaTriTimKiemLower[sizeof(giaTriTimKiemLower) - 1] = '\0';
    }


    for (int i = 0; i < soLuongLanKham; ++i) {
        int timThayTrongLanKhamNay = 0; // Bien co de danh dau tim thay cho lan kham hien tai

        if (strcmp(tieuChiLower, "malankham") == 0) {
            if (dsLanKham[i].maLanKham == atoi(giaTriTimKiemLower)) { // atoi se bo qua cac ky tu khong phai so
                timThayTrongLanKhamNay = 1;
            }
        }
        else if (strcmp(tieuChiLower, "madinhdanhbn") == 0) {
            char currentMaDDBNLower[MAX_LEN_MA_DINH_DANH_BN + 1];
            strncpy(currentMaDDBNLower, dsLanKham[i].maDinhDanh, MAX_LEN_MA_DINH_DANH_BN);
            currentMaDDBNLower[MAX_LEN_MA_DINH_DANH_BN] = '\0';
            for (int k = 0; currentMaDDBNLower[k]; k++) currentMaDDBNLower[k] = tolower(currentMaDDBNLower[k]);
            // Su dung strstr de tim kiem mot phan, hoac strcmp neu muon khop chinh xac
            if (strcmp(currentMaDDBNLower, giaTriTimKiemLower) == 0) {
                timThayTrongLanKhamNay = 1;
            }
        }
        else if (strcmp(tieuChiLower, "trangthai") == 0) {
            // SUA LOI TAI DAY: Loai bo khoi lenh if bi lap va trong
            char currentTrangThaiLower[50];
            // Lay chuoi trang thai tu enum, sau do chuyen sang chu thuong
            strncpy(currentTrangThaiLower, trangThaiLanKhamToString(dsLanKham[i].trangThai), sizeof(currentTrangThaiLower) - 1);
            currentTrangThaiLower[sizeof(currentTrangThaiLower) - 1] = '\0';
            for (int k = 0; currentTrangThaiLower[k]; k++) {
                currentTrangThaiLower[k] = tolower(currentTrangThaiLower[k]);
            }

            // So sanh chuoi trang thai hien tai (da chuyen sang chu thuong)
            // voi giaTriTimKiemLower (da duoc chuyen sang chu thuong o tren)
            if (strcmp(currentTrangThaiLower, giaTriTimKiemLower) == 0) {
                timThayTrongLanKhamNay = 1;
            }
        }
        else if (strcmp(tieuChiLower, "bacsi") == 0) {
            char currentMaBacSiLower[MAX_LEN_MA_BAC_SI + 1];
            strncpy(currentMaBacSiLower, dsLanKham[i].maBacSi, MAX_LEN_MA_BAC_SI);
            currentMaBacSiLower[MAX_LEN_MA_BAC_SI] = '\0';
            for (int k = 0; currentMaBacSiLower[k]; k++) {
                currentMaBacSiLower[k] = tolower(currentMaBacSiLower[k]);
            }
            // Su dung strstr de tim kiem neu ma bac si trong dsLanKham co chua giaTriTimKiemLower
            if (strstr(currentMaBacSiLower, giaTriTimKiemLower) != NULL) {
                timThayTrongLanKhamNay = 1;
            }
        }
        // Cac tieu chi khac co the them o day (vi du: theo ngay kham, theo khoa phong)

        if (timThayTrongLanKhamNay) {
            dsKQTemp[count++] = dsLanKham[i]; // Them vao danh sach ket qua tam thoi
        }
    }

    if (count > 0) {
        *ketQuaTimKiem = (LanKham*)malloc(count * sizeof(LanKham)); // Cap phat bo nho dung cho so luong ket qua
        if (*ketQuaTimKiem != NULL) {
            memcpy(*ketQuaTimKiem, dsKQTemp, count * sizeof(LanKham)); // Sao chep ket qua
            *soLuongKetQua = count;
        }
        else {
            perror("Loi malloc cho ketQuaTimKiem trong timKiemLanKham");
            *soLuongKetQua = 0; // Reset so luong ket qua neu cap phat loi
        }
    }
    free(dsKQTemp); // Giai phong bo nho tam thoi
}
// Trong file examination.c

// (Cac include va cac ham khac cua examination.c giu nguyen)
// ...

int suaChiTietLanKham(LanKham dsLanKham[], int soLuongLanKham, int maLanKhamCanSua,
    const BacSi dsBacSi[], int soLuongBacSi,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
    const char* fileLanKhamCSV) {
    int indexLK = -1;
    for (int i = 0; i < soLuongLanKham; ++i) {
        if (dsLanKham[i].maLanKham == maLanKhamCanSua) {
            indexLK = i;
            break;
        }
    }

    if (indexLK == -1) {
        printf("LOI: Khong tim thay Lan Kham voi Ma: %d de sua.\n", maLanKhamCanSua);
        return 0;
    }

    LanKham* lkCanSua = &dsLanKham[indexLK];
    LanKham lkTamSua = *lkCanSua;

    printf("\n--- SUA CHI TIET LAN KHAM (Ma LK: %d, Benh Nhan: %s, Trang Thai: %s) ---\n",
        lkTamSua.maLanKham, lkTamSua.tenBenhNhan, trangThaiLanKhamToString(lkTamSua.trangThai));

    int luaChonSuaNoiBo;
    char bufferNhap[256];
    char maKhoaPhongTheoBacSiTam[sizeof(lkTamSua.maKhoaPhong)] = "";

    do {
        printf("\nChon thong tin muon sua:\n");
        printf("  1. Ma Bac Si        (Hien tai: %s)\n", strlen(lkTamSua.maBacSi) > 0 ? lkTamSua.maBacSi : "CHUA_GAN");
        printf("  2. Ma Khoa Phong    (Hien tai: %s)\n", strlen(lkTamSua.maKhoaPhong) > 0 ? lkTamSua.maKhoaPhong : "CHUA_GAN"); // Giu nguyen logic cho khoa phong
        printf("  3. Chan Doan        (Hien tai: %s)\n", strlen(lkTamSua.chanDoan) > 0 ? lkTamSua.chanDoan : "CHUA_XAC_DINH");
        printf("  4. Phuong Phap Dieu Tri (Hien tai: %s)\n", strlen(lkTamSua.phuongPhapDieuTri) > 0 ? lkTamSua.phuongPhapDieuTri : "CHUA_XAC_DINH");

        // Hien thi ghiChuBacSi - Neu "-1" hoac rong, co the hien thi "Khong" de goi y
        char ghiChuHienTaiSua[sizeof(lkTamSua.ghiChuBacSi) + 1];
        if (strcmp(lkTamSua.ghiChuBacSi, "-1") == 0 || strlen(lkTamSua.ghiChuBacSi) == 0) {
            strcpy(ghiChuHienTaiSua, "Khong"); // Goi y rang neu khong co thi se la "Khong"
        }
        else {
            strncpy(ghiChuHienTaiSua, lkTamSua.ghiChuBacSi, sizeof(ghiChuHienTaiSua) - 1);
            ghiChuHienTaiSua[sizeof(ghiChuHienTaiSua) - 1] = '\0';
        }
        printf("  5. Ghi Chu Bac Si   (Hien tai: %s)\n", ghiChuHienTaiSua);
        char ngayTaiKhamStr_ui[12] = "-";
        if (lkTamSua.coLichTaiKham && lkTamSua.ngayTaiKham.ngay > 0) {
            snprintf(ngayTaiKhamStr_ui, sizeof(ngayTaiKhamStr_ui), "%02d/%02d/%04d", lkTamSua.ngayTaiKham.ngay, lkTamSua.ngayTaiKham.thang, lkTamSua.ngayTaiKham.nam);
        }
        printf("  6. Ngay Tai Kham    (Hien tai: %s, Co lich: %s)\n", ngayTaiKhamStr_ui, lkTamSua.coLichTaiKham ? "Co" : "Khong");
        printf("-------------------------------------------------\n");
        printf("  0. HOAN TAT SUA DOI VA LUU\n");
        printf("  9. Huy bo tat ca thay doi va quay lai\n");

        luaChonSuaNoiBo = nhapSoNguyenCoGioiHan("Lua chon cua ban", 0, 9);
        if (luaChonSuaNoiBo < 0) { luaChonSuaNoiBo = 9; }

        switch (luaChonSuaNoiBo) {
        case 1: {
            printf("Nhap Ma Bac Si moi (khong de trong, hien tai: %s): ", strlen(lkTamSua.maBacSi) > 0 ? lkTamSua.maBacSi : "CHUA_GAN");
            docDongAnToan(bufferNhap, sizeof(lkTamSua.maBacSi), stdin);
            if (strlen(bufferNhap) > 0) { // Nguoi dung co nhap
                for (int i = 0; bufferNhap[i]; i++) bufferNhap[i] = toupper(bufferNhap[i]);
                const BacSi* bsTimThay = timBacSiTheoMa(dsBacSi, soLuongBacSi, bufferNhap);
                if (bsTimThay != NULL) {
                    strcpy(lkTamSua.maBacSi, bsTimThay->maBacSi);
                    // ... (xu ly maKhoaPhongTheoBacSiTam) ...
                }
                else {
                    printf("CANH BAO: Ma Bac Si '%s' khong tim thay. Vui long nhap lai ma hop le.\n", bufferNhap);
                    // Khong thay doi lkTamSua.maBacSi, cho phep nguoi dung sua lai o lan sau hoac huy
                }
            }
            else { // Nguoi dung bo trong
                printf("LOI: Ma Bac Si khong duoc de trong. Gia tri chua duoc thay doi.\n");
            }
            break;
        }
        case 2: {
            if (strlen(maKhoaPhongTheoBacSiTam) == 0 && strlen(lkTamSua.maBacSi) > 0 && strcmp(lkTamSua.maBacSi, "CHUA_GAN") != 0) {
                const BacSi* bsHienTai = timBacSiTheoMa(dsBacSi, soLuongBacSi, lkTamSua.maBacSi);
                if (bsHienTai != NULL && strlen(bsHienTai->maKhoaPhong) > 0) {
                    strcpy(maKhoaPhongTheoBacSiTam, bsHienTai->maKhoaPhong);
                }
            }

            printf("Nhap Ma Khoa Phong moi (hien tai: %s, theo bac si: %s).\nNhan Enter de dung Ma KP theo bac si (neu co va hop le), hoac nhap moi: ",
                strlen(lkTamSua.maKhoaPhong) > 0 ? lkTamSua.maKhoaPhong : "CHUA_GAN",
                strlen(maKhoaPhongTheoBacSiTam) > 0 ? maKhoaPhongTheoBacSiTam : "KHONG_CO");
            docDongAnToan(bufferNhap, sizeof(lkTamSua.maKhoaPhong), stdin);

            if (strlen(bufferNhap) > 0) { // Nguoi dung nhap ma khoa phong moi
                for (int i = 0; bufferNhap[i]; i++) bufferNhap[i] = toupper(bufferNhap[i]);
                if (timKhoaPhongTheoMa(dsKhoaPhong, soLuongKhoaPhong, bufferNhap) != NULL) {
                    strcpy(lkTamSua.maKhoaPhong, bufferNhap);
                    printf("Da cap nhat Ma Khoa Phong thanh: %s\n", lkTamSua.maKhoaPhong);
                }
                else {
                    printf("CANH BAO: Ma Khoa Phong moi '%s' khong tim thay.\n", bufferNhap);
                    if (strlen(maKhoaPhongTheoBacSiTam) > 0 && timKhoaPhongTheoMa(dsKhoaPhong, soLuongKhoaPhong, maKhoaPhongTheoBacSiTam) != NULL) {
                        strcpy(lkTamSua.maKhoaPhong, maKhoaPhongTheoBacSiTam);
                        printf("Da su dung Ma Khoa Phong theo bac si: %s\n", lkTamSua.maKhoaPhong);
                    }
                    else {
                        printf("Ma Khoa Phong khong duoc thay doi (Ma nhap moi khong hop le va khong co Ma KP hop le theo bac si).\n");
                    }
                }
            }
            else { // Nguoi dung bo trong
                if (strlen(maKhoaPhongTheoBacSiTam) > 0 && timKhoaPhongTheoMa(dsKhoaPhong, soLuongKhoaPhong, maKhoaPhongTheoBacSiTam) != NULL) {
                    strcpy(lkTamSua.maKhoaPhong, maKhoaPhongTheoBacSiTam);
                    printf("Da su dung Ma Khoa Phong theo bac si: %s\n", lkTamSua.maKhoaPhong);
                }
                else {
                    printf("Khong co Ma Khoa Phong theo bac si hoac ma do khong hop le. Ma Khoa Phong khong thay doi.\n");
                }
            }
            break;
        }
        case 3:
            printf("Nhap Chan Doan moi (khong de trong, hien tai: %s): ", strlen(lkTamSua.chanDoan) > 0 ? lkTamSua.chanDoan : "CHUA_XAC_DINH");
            docDongAnToan(lkTamSua.chanDoan, sizeof(lkTamSua.chanDoan), stdin);
            if (strlen(lkTamSua.chanDoan) == 0) {
                printf("LOI: Chan Doan khong duoc de trong. Vui long nhap lai hoac huy.\n");
                // De giu gia tri cu neu nguoi dung khong muon thay doi nua:
                // strcpy(lkTamSua.chanDoan, lkCanSua->chanDoan); // Hoac dat 1 gia tri tam thoi de nhac nho
            }
            break;
        case 4:
            printf("Nhap Phuong Phap Dieu Tri moi (khong de trong, hien tai: %s): ", strlen(lkTamSua.phuongPhapDieuTri) > 0 ? lkTamSua.phuongPhapDieuTri : "CHUA_XAC_DINH");
            docDongAnToan(lkTamSua.phuongPhapDieuTri, sizeof(lkTamSua.phuongPhapDieuTri), stdin);
            if (strlen(lkTamSua.phuongPhapDieuTri) == 0) {
                printf("LOI: Phuong Phap Dieu Tri khong duoc de trong. Vui long nhap lai hoac huy.\n");
            }
            break;
        case 5:
            if (lkTamSua.trangThai == LK_DA_HOAN_THANH) {
                char ghiChuHienTaiDisplayCase5[sizeof(lkTamSua.ghiChuBacSi) + 1];
                if (strcmp(lkTamSua.ghiChuBacSi, "-1") == 0 || strlen(lkTamSua.ghiChuBacSi) == 0) { // Logic hien thi cu
                    strcpy(ghiChuHienTaiDisplayCase5, "N/A");
                }
                else {
                    strncpy(ghiChuHienTaiDisplayCase5, lkTamSua.ghiChuBacSi, sizeof(ghiChuHienTaiDisplayCase5) - 1);
                    ghiChuHienTaiDisplayCase5[sizeof(ghiChuHienTaiDisplayCase5) - 1] = '\0';
                }
                printf("Nhap Ghi Chu Bac Si moi (hien tai: %s, de trong se luu la 'Khong', hoac nhap 'Khong'): ", ghiChuHienTaiDisplayCase5);
                docDongAnToan(lkTamSua.ghiChuBacSi, sizeof(lkTamSua.ghiChuBacSi), stdin);
                // Viec xu ly gia tri "" thanh "Khong" se thuc hien o case 0
            }
            else {
                printf("Ghi chu bac si se duoc dat la 'Khong' (luu vao CSV la 'Khong') cho trang thai '%s'.\n",
                    trangThaiLanKhamToString(lkTamSua.trangThai));
                strcpy(lkTamSua.ghiChuBacSi, "Khong"); // Dat gia tri trong bo nho tam thoi la "Khong"
            }
            break;
        case 6: { // Sua Ngay Tai Kham
            char taiKhamChoice_str[10];
            printf("Co thay doi lich tai kham khong? (c/k, hien tai: %s): ", lkTamSua.coLichTaiKham ? "Co" : "Khong");
            docDongAnToan(taiKhamChoice_str, sizeof(taiKhamChoice_str), stdin);
            if (strlen(taiKhamChoice_str) > 0 && tolower(taiKhamChoice_str[0]) == 'c') {
                lkTamSua.coLichTaiKham = 1;
                printf("Nhap Ngay Tai Kham moi (ngay thang nam, vd: 15 01 2025): ");
                if (docDongAnToan(bufferNhap, sizeof(bufferNhap), stdin) != NULL &&
                    sscanf(bufferNhap, "%d %d %d", &lkTamSua.ngayTaiKham.ngay, &lkTamSua.ngayTaiKham.thang, &lkTamSua.ngayTaiKham.nam) == 3) {
                    // TODO: Them kiem tra ngay thang nam hop le o day neu can
                    printf("Da cap nhat ngay tai kham.\n");
                }
                else {
                    printf("Ngay tai kham nhap khong hop le. Huy thay doi lich tai kham.\n");
                    lkTamSua.coLichTaiKham = dsLanKham[indexLK].coLichTaiKham; // Hoan lai gia tri cu
                    lkTamSua.ngayTaiKham = dsLanKham[indexLK].ngayTaiKham;
                }
            }
            else if (strlen(taiKhamChoice_str) > 0 && tolower(taiKhamChoice_str[0]) == 'k') {
                lkTamSua.coLichTaiKham = 0;
                lkTamSua.ngayTaiKham.ngay = -1; lkTamSua.ngayTaiKham.thang = -1; lkTamSua.ngayTaiKham.nam = -1;
                printf("Da huy lich tai kham.\n");
            }
            else {
                printf("Lua chon khong hop le. Thong tin tai kham khong thay doi.\n");
            }
            break;
        }
        case 0: // Hoan tat va Luu
            // Kiem tra cac truong khong duoc de rong, tuy thuoc vao trang thai
            // Neu lan kham da hoan thanh, cac truong nay la bat buoc
            if (lkTamSua.trangThai == LK_DA_HOAN_THANH) {
                if (strlen(lkTamSua.maBacSi) == 0 || strcmp(lkTamSua.maBacSi, "CHUA_GAN") == 0) {
                    printf("LOI (Lan Kham Da Hoan Thanh): Ma Bac Si khong duoc de trong hoac CHUA_GAN. Vui long cap nhat.\n");
                    luaChonSuaNoiBo = -1; // De vong lap tiep tuc, khong luu
                    break;
                }
                if (strlen(lkTamSua.chanDoan) == 0 || strcmp(lkTamSua.chanDoan, "CHUA_KHAM") == 0 || strcmp(lkTamSua.chanDoan, "CHUA_XAC_DINH") == 0 || strcmp(lkTamSua.chanDoan, "CHUA_NHAP_CD") == 0) {
                    printf("LOI (Lan Kham Da Hoan Thanh): Chan Doan khong duoc de trong hoac o gia tri mac dinh. Vui long cap nhat.\n");
                    luaChonSuaNoiBo = -1;
                    break;
                }
                if (strlen(lkTamSua.phuongPhapDieuTri) == 0 || strcmp(lkTamSua.phuongPhapDieuTri, "CHUA_KHAM") == 0 || strcmp(lkTamSua.phuongPhapDieuTri, "CHUA_XAC_DINH") == 0 || strcmp(lkTamSua.phuongPhapDieuTri, "CHUA_NHAP_PPTT") == 0) {
                    printf("LOI (Lan Kham Da Hoan Thanh): Phuong Phap Dieu Tri khong duoc de trong hoac o gia tri mac dinh. Vui long cap nhat.\n");
                    luaChonSuaNoiBo = -1;
                    break;
                }
            }
            // Neu khong phai LK_DA_HOAN_THANH, khong bat buoc cac truong tren o muc do nghiem ngat nay trong chuc nang "Sua Chi Tiet"
            // Tuy nhien, ban van co the them canh bao neu muon.

            // Xu ly ghiChuBacSi de luu vao CSV
            if (lkTamSua.trangThai != LK_DA_HOAN_THANH) {
                // Neu khong phai DaHoanThanh, va ghiChuBacSi la "-1" (chua bao gio nhap),
                // hoac de trong khi sua, thi co the giu nguyen "-1" hoac dat la "Khong" tuy theo quy dinh.
                // Neu nguoi dung da nhap gi do, thi giu nguyen.
                // O day, ta giu logic cu la dat thanh "Khong" neu khong phai DaHoanThanh.
                strcpy(lkTamSua.ghiChuBacSi, "Khong");
            }
            else { // Neu la LK_DA_HOAN_THANH
                char ghiChuTamLowerSua[sizeof(lkTamSua.ghiChuBacSi)];
                strncpy(ghiChuTamLowerSua, lkTamSua.ghiChuBacSi, sizeof(ghiChuTamLowerSua) - 1); //
                ghiChuTamLowerSua[sizeof(ghiChuTamLowerSua) - 1] = '\0'; //
                for (int k = 0; ghiChuTamLowerSua[k]; ++k) ghiChuTamLowerSua[k] = tolower(ghiChuTamLowerSua[k]); //

                if (strlen(lkTamSua.ghiChuBacSi) == 0) { // Neu nguoi dung de trong //
                    strcpy(lkTamSua.ghiChuBacSi, "Khong"); // Luu la "Khong" //
                }
                else if (strcmp(ghiChuTamLowerSua, "khong") == 0) { // Neu nguoi dung nhap "khong" (bat ky hoa/thuong) //
                    strcpy(lkTamSua.ghiChuBacSi, "Khong"); // Chuan hoa thanh "Khong" co viet hoa chu K //
                }
                // Neu nguoi dung nhap mot ghi chu khac, thi giu nguyen ghi chu do
            }

            *lkCanSua = lkTamSua;
            ghiDanhSachLanKhamRaFile(fileLanKhamCSV, dsLanKham, soLuongLanKham);
            printf("THANH CONG: Da luu cac thay doi cho Lan Kham Ma: %d.\n", maLanKhamCanSua);
            return 1;
        case 9: // Huy bo
            printf("Da huy bo tat ca thay doi.\n");
            return 0; // Thoat khoi ham
        default:
            printf("LOI: Lua chon khong hop le.\n");
            break;
        }
        if (luaChonSuaNoiBo >= 1 && luaChonSuaNoiBo <= 6) {
            printf("Thong tin da duoc cap nhat tam thoi. Chon truong khac de sua hoac Luu/Huy.\n");
            dungManHinhCho(); // Gia su ban co ham nay
        }
        xoaManHinhConsole(); // Xoa man hinh cho lan lap menu sua tiep theo
        printf("\n--- SUA CHI TIET LAN KHAM (Ma LK: %d, Benh Nhan: %s, Trang Thai: %s) ---\n",
            lkTamSua.maLanKham, lkTamSua.tenBenhNhan, trangThaiLanKhamToString(lkTamSua.trangThai));
    } while (luaChonSuaNoiBo != 0 && luaChonSuaNoiBo != 9);

    return 0; // Neu thoat vong lap ma khong phai do luu hoac huy ro rang (it khi xay ra)
}

// ... (Phan con lai cua examination.c)