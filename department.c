#include "department.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Cho toupper

// Ham tien ich noi bo (static)
static void removeNewlineDept(char* str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

static void readStringSafeDept(char* buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        removeNewlineDept(buffer);
    } else {
        buffer[0] = '\0';
    }
}

KhoaPhong* docDanhSachKhoaPhong(const char* tenFile, int* soLuong) {
    FILE* f = fopen(tenFile, "r");
    if (f == NULL) {
        // perror("Loi mo file de doc (docDanhSachKhoaPhong)"); // Co the khong in loi neu file chua ton tai
        *soLuong = 0;
        return NULL; // Tra ve NULL neu file khong ton tai, coi nhu danh sach rong
    }

    KhoaPhong* ds = NULL;
    int capacity = 0;
    *soLuong = 0;
    char line[500]; // Buffer cho mot dong CSV khoa phong

    // Bo qua dong tieu de (neu co)
    // fgets(line, sizeof(line), f); 

    while (fgets(line, sizeof(line), f) != NULL) {
        removeNewlineDept(line);
        if (strlen(line) < 3) continue; // Dong qua ngan

        if (*soLuong >= capacity) {
            capacity = (capacity == 0) ? 5 : capacity * 2; // Khoi tao voi 5 khoa phong
            KhoaPhong* temp = realloc(ds, capacity * sizeof(KhoaPhong));
            if (temp == NULL) {
                perror("Loi cap phat bo nho (realloc docDanhSachKhoaPhong)");
                free(ds);
                fclose(f);
                *soLuong = 0;
                return NULL;
            }
            ds = temp;
        }

        KhoaPhong kp;
        // Dinh dang CSV: maKhoaPhong,tenKhoaPhong,viTri,moTa
        char* token;
        
        token = strtok(line, ","); if (token) strncpy(kp.maKhoaPhong, token, sizeof(kp.maKhoaPhong)-1); else continue;
        kp.maKhoaPhong[sizeof(kp.maKhoaPhong)-1] = '\0';

        token = strtok(NULL, ","); if (token) strncpy(kp.tenKhoaPhong, token, sizeof(kp.tenKhoaPhong)-1); else strcpy(kp.tenKhoaPhong, "");
        kp.tenKhoaPhong[sizeof(kp.tenKhoaPhong)-1] = '\0';
        
        token = strtok(NULL, ","); if (token) strncpy(kp.viTri, token, sizeof(kp.viTri)-1); else strcpy(kp.viTri, "");
        kp.viTri[sizeof(kp.viTri)-1] = '\0';

        token = strtok(NULL, "\n"); // Doc den het dong cho mo ta, vi mo ta co the chua dau phay
                                    // Neu muon mo ta khong chua dau phay thi dung "," nhu tren
        if (token) strncpy(kp.moTa, token, sizeof(kp.moTa)-1); else strcpy(kp.moTa, "");
        kp.moTa[sizeof(kp.moTa)-1] = '\0';
        
        ds[*soLuong] = kp;
        (*soLuong)++;
    }

    fclose(f);
    return ds;
}

void ghiDanhSachKhoaPhongRaFile(const char* tenFile, const KhoaPhong dsKhoaPhong[], int soLuong) {
    FILE* f = fopen(tenFile, "w"); // Mo o che do "w" de ghi de
    if (f == NULL) {
        perror("Loi mo file de ghi (ghiDanhSachKhoaPhongRaFile)");
        return;
    }

    // Ghi dong tieu de (tuy chon)
    // fprintf(f, "MaKhoaPhong,TenKhoaPhong,ViTri,MoTa\n");

    for (int i = 0; i < soLuong; ++i) {
        fprintf(f, "%s,%s,%s,%s\n",
                dsKhoaPhong[i].maKhoaPhong,
                dsKhoaPhong[i].tenKhoaPhong,
                dsKhoaPhong[i].viTri,
                dsKhoaPhong[i].moTa);
    }
    fclose(f);
}

KhoaPhong* timKhoaPhongTheoMa(const KhoaPhong dsKhoaPhong[], int soLuong, const char* maKhoaPhong) {
    if (maKhoaPhong == NULL) return NULL;
    for (int i = 0; i < soLuong; ++i) {
        if (strcmp(dsKhoaPhong[i].maKhoaPhong, maKhoaPhong) == 0) {
            return (KhoaPhong*)&dsKhoaPhong[i]; // Tra ve con tro toi phan tu trong mang
        }
    }
    return NULL; // Khong tim thay
}

int nhapThongTinKhoaPhongTuBanPhim(KhoaPhong* kp, const KhoaPhong dsKhoaPhong[], int soLuongHienCo) {
    char bufferMa[sizeof(kp->maKhoaPhong)];
    printf("Nhap Ma Khoa Phong (VD: KHOA_TIMMACH, toi da %zu ky tu): ", sizeof(kp->maKhoaPhong)-1);
    readStringSafeDept(bufferMa, sizeof(bufferMa));
    // Chuyen ma khoa phong sang chu hoa de dam bao tinh nhat quan
    for(int i = 0; bufferMa[i]; i++){
        bufferMa[i] = toupper(bufferMa[i]);
    }

    if (strlen(bufferMa) == 0) {
        printf("Ma Khoa Phong khong duoc de trong.\n");
        return 0;
    }
    if (timKhoaPhongTheoMa(dsKhoaPhong, soLuongHienCo, bufferMa) != NULL) {
        printf("Loi: Ma Khoa Phong '%s' da ton tai!\n", bufferMa);
        return 0; // Ma da ton tai
    }
    strncpy(kp->maKhoaPhong, bufferMa, sizeof(kp->maKhoaPhong)-1);
    kp->maKhoaPhong[sizeof(kp->maKhoaPhong)-1] = '\0';

    printf("Nhap Ten Khoa Phong: "); readStringSafeDept(kp->tenKhoaPhong, sizeof(kp->tenKhoaPhong));
    printf("Nhap Vi Tri: "); readStringSafeDept(kp->viTri, sizeof(kp->viTri));
    printf("Nhap Mo Ta: "); readStringSafeDept(kp->moTa, sizeof(kp->moTa));
    return 1; // Nhap thanh cong
}


int themKhoaPhong(KhoaPhong** dsKhoaPhongConTro, int* soLuongConTro, KhoaPhong khoaPhongMoi, const char* tenFile) {
    // Kiem tra trung ma truoc khi them (mac du nhapThongTin... da kiem tra, an toan hon khi kiem tra lai)
    if (timKhoaPhongTheoMa(*dsKhoaPhongConTro, *soLuongConTro, khoaPhongMoi.maKhoaPhong) != NULL) {
        // printf("Loi: Ma Khoa Phong '%s' da ton tai khi them.\n", khoaPhongMoi.maKhoaPhong);
        // Ham nhapThongTin... se xu ly viec nay, nen dong nay co the khong can thiet
        return 0; 
    }

    KhoaPhong* temp = realloc(*dsKhoaPhongConTro, (*soLuongConTro + 1) * sizeof(KhoaPhong));
    if (temp == NULL) {
        perror("Loi cap phat bo nho (realloc themKhoaPhong)");
        return 0; // That bai
    }
    *dsKhoaPhongConTro = temp;

    (*dsKhoaPhongConTro)[*soLuongConTro] = khoaPhongMoi; // Gan du lieu
    (*soLuongConTro)++;

    ghiDanhSachKhoaPhongRaFile(tenFile, *dsKhoaPhongConTro, *soLuongConTro);
    return 1; // Thanh cong
}

int suaThongTinKhoaPhong(KhoaPhong dsKhoaPhong[], int soLuong, const char* maKhoaPhongCanSua, KhoaPhong thongTinMoi, const char* tenFile) {
    int timThay = 0;
    for (int i = 0; i < soLuong; ++i) {
        if (strcmp(dsKhoaPhong[i].maKhoaPhong, maKhoaPhongCanSua) == 0) {
            // Giu lai maKhoaPhong goc, chi cap nhat cac thong tin khac
            // Ma khoa phong khong nen thay doi, neu muon thay doi ma, can xoa roi them moi
            strcpy(thongTinMoi.maKhoaPhong, dsKhoaPhong[i].maKhoaPhong); 
            dsKhoaPhong[i] = thongTinMoi;
            timThay = 1;
            break;
        }
    }

    if (timThay) {
        ghiDanhSachKhoaPhongRaFile(tenFile, dsKhoaPhong, soLuong);
        return 1; // Sua thanh cong
    }
    return 0; // Khong tim thay
}

int xoaKhoaPhong(KhoaPhong** dsKhoaPhongConTro, int* soLuongConTro, const char* maKhoaPhongCanXoa, const char* tenFile) {
    int viTriCanXoa = -1;
    for (int i = 0; i < *soLuongConTro; ++i) {
        if (strcmp((*dsKhoaPhongConTro)[i].maKhoaPhong, maKhoaPhongCanXoa) == 0) {
            viTriCanXoa = i;
            break;
        }
    }

    if (viTriCanXoa != -1) {
        // Dich chuyen cac phan tu phia sau len
        for (int i = viTriCanXoa; i < (*soLuongConTro - 1); ++i) {
            (*dsKhoaPhongConTro)[i] = (*dsKhoaPhongConTro)[i + 1];
        }
        (*soLuongConTro)--;

        if (*soLuongConTro == 0) {
            free(*dsKhoaPhongConTro);
            *dsKhoaPhongConTro = NULL;
        } else {
            KhoaPhong* temp = realloc(*dsKhoaPhongConTro, (*soLuongConTro) * sizeof(KhoaPhong));
            if (temp == NULL && *soLuongConTro > 0) {
                // Loi realloc khi thu nho, nhung du lieu van con, chi la ton bo nho
                perror("Loi realloc khi xoa khoa phong (thu nho mang)");
            } else if (*soLuongConTro > 0) {
                *dsKhoaPhongConTro = temp;
            }
        }
        ghiDanhSachKhoaPhongRaFile(tenFile, *dsKhoaPhongConTro, *soLuongConTro);
        return 1; // Xoa thanh cong
    }
    return 0; // Khong tim thay
}

void inDanhSachKhoaPhongConsole(const KhoaPhong dsKhoaPhong[], int soLuong) {
    printf("\n--- DANH SACH KHOA PHONG ---\n");
    if (soLuong == 0) {
        printf("Khong co khoa phong nao trong danh sach.\n");
        return;
    }
    printf("-------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-30s | %-20s | %-30s |\n", "STT", "Ma Khoa Phong", "Ten Khoa Phong", "Vi Tri", "Mo Ta");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < soLuong; ++i) {
        printf("| %-5d | %-20s | %-30s | %-20s | %-30.30s |\n", // Cat bot mo ta neu qua dai
               i + 1,
               dsKhoaPhong[i].maKhoaPhong,
               dsKhoaPhong[i].tenKhoaPhong,
               dsKhoaPhong[i].viTri,
               dsKhoaPhong[i].moTa);
    }
    printf("-------------------------------------------------------------------------------------------------------------------------\n");
}