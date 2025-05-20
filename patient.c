    #include "patient.h"
    #include "csv_handler.h" // Cho docDongAnToan
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <time.h>   
    #include <ctype.h>  
    #define PATIENT_HO_TEN_COL_WIDTH 30
    #define PATIENT_DIA_CHI_COL_WIDTH 30
    #define PATIENT_TIEU_SU_COL_WIDTH 20
    // static void removeNewline(char* str) { /* ... */ } // Giu nguyen neu co
    // static void readStringSafe(char* buffer, int size) { /* ... */ } // Co the thay bang docDongAnToan
    
    const char* gioiTinhToString(GioiTinh gt) {
        switch (gt) {
            case GT_NAM: return "Nam";
            case GT_NU: return "Nu";
            case GT_KHAC: return "Khac";
            default: return "KhongXacDinh";
        }
    }
    
    GioiTinh stringToGioiTinh(const char* str) {
        if (str == NULL || strlen(str) == 0) return GT_KHAC;    
        char lowerStr[100];
        strncpy(lowerStr, str, 99);
        lowerStr[99] = '\0';
        for(int i = 0; lowerStr[i]; i++){
            lowerStr[i] = tolower(lowerStr[i]);
        }
    
        if (strcmp(lowerStr, "nam") == 0) return GT_NAM;
        if (strcmp(lowerStr, "nu") == 0) return GT_NU;
        if (strcmp(lowerStr, "khac") == 0) return GT_KHAC;
        return GT_KHAC; 
    }
    
    
    BenhNhan* docDanhSachBenhNhan(const char* tenFile, int* soLuongBenhNhan) {
        // ... (Ham nay giu nguyen logic doc tu file)
        // ... (Copy lai toan bo ham docDanhSachBenhNhan tu phien ban truoc)
        FILE* f = fopen(tenFile, "r");
        if (f == NULL) {
            *soLuongBenhNhan = 0;
            return NULL;
        }

        BenhNhan* ds = NULL;
        int capacity = 0;
        *soLuongBenhNhan = 0;
        char line[1500]; 

        while (fgets(line, sizeof(line), f) != NULL) {
            xoaKyTuXuongDong(line); // Su dung ham tu csv_handler.h
            if (strlen(line) < 10) continue; 

            if (*soLuongBenhNhan >= capacity) {
                capacity = (capacity == 0) ? 10 : capacity * 2; 
                BenhNhan* temp = realloc(ds, capacity * sizeof(BenhNhan));
                if (temp == NULL) {
                    printf("Loi cap phat bo nho (realloc docDanhSachBenhNhan)");
                    free(ds); 
                    fclose(f);
                    *soLuongBenhNhan = 0; 
                    return NULL;
                }
                ds = temp;
            }

            BenhNhan bn;
            char gioiTinhStr[20];
            long ngayTaoTimestamp;
            char* token;
            
            token = strtok(line, ","); 
            if (token) {
                strncpy(bn.maDinhDanh, token, sizeof(bn.maDinhDanh)-1);
            bn.maDinhDanh[sizeof(bn.maDinhDanh)-1] = '\0';
            } else {
                // Ma dinh danh la bat buoc, bo qua dong neu khong co
                fprintf(stderr, "Loi doc CSV Benh Nhan: Dong khong co Ma Dinh Danh.\n");
            continue; 
            }
            token = strtok(NULL, ","); if (token) strncpy(bn.bhyt, token, sizeof(bn.bhyt)-1); else strcpy(bn.bhyt, ""); bn.bhyt[sizeof(bn.bhyt)-1] = '\0';
            token = strtok(NULL, ","); if (token) strncpy(bn.soDienThoai, token, sizeof(bn.soDienThoai)-1); else strcpy(bn.soDienThoai, ""); bn.soDienThoai[sizeof(bn.soDienThoai)-1] = '\0';
            token = strtok(NULL, ","); if (token) strncpy(bn.ten, token, sizeof(bn.ten)-1); else strcpy(bn.ten, ""); bn.ten[sizeof(bn.ten)-1] = '\0';
            token = strtok(NULL, ","); if (token) bn.ngaySinh.ngay = atoi(token); else bn.ngaySinh.ngay = 0;
            token = strtok(NULL, ","); if (token) bn.ngaySinh.thang = atoi(token); else bn.ngaySinh.thang = 0;
            token = strtok(NULL, ","); if (token) bn.ngaySinh.nam =  atoi(token); else bn.ngaySinh.nam = 0;
            token = strtok(NULL, ","); if (token) bn.tuoi = atoi(token); else bn.tuoi = 0;
            token = strtok(NULL, ","); if (token) strncpy(gioiTinhStr, token, sizeof(gioiTinhStr)-1); else strcpy(gioiTinhStr, ""); gioiTinhStr[sizeof(gioiTinhStr)-1] = '\0';
                                        bn.gioiTinh = stringToGioiTinh(gioiTinhStr);
            token = strtok(NULL, ","); if (token) strncpy(bn.diaChi, token, sizeof(bn.diaChi)-1); else strcpy(bn.diaChi, ""); bn.diaChi[sizeof(bn.diaChi)-1] = '\0';
            token = strtok(NULL, ","); if (token) strncpy(bn.tieuSuBenhLy, token, sizeof(bn.tieuSuBenhLy)-1); else strcpy(bn.tieuSuBenhLy, ""); bn.tieuSuBenhLy[sizeof(bn.tieuSuBenhLy)-1] = '\0';
            token = strtok(NULL, "\n"); if (token) ngayTaoTimestamp = atol(token); else ngayTaoTimestamp = 0; // Doc den het dong
                                        bn.ngayTaoHoSo = (time_t)ngayTaoTimestamp;

            ds[*soLuongBenhNhan] = bn;
            (*soLuongBenhNhan)++;
        }
        fclose(f);
        return ds;
    }
    
    void ghiDanhSachBenhNhanRaFile(const char* tenFile, const BenhNhan dsBenhNhan[], int soLuongBenhNhan) {
        // ... (Ham nay giu nguyen logic ghi ra file)
        // ... (Copy lai toan bo ham ghiDanhSachBenhNhanRaFile tu phien ban truoc)
        FILE* f = fopen(tenFile, "w"); 
        if (f == NULL) {
            perror("Loi mo file de ghi (ghiDanhSachBenhNhanRaFile)");
            return;
        }
        for (int i = 0; i < soLuongBenhNhan; ++i) {
            fprintf(f, "%s,%s,%s,%s,%d,%d,%d,%d,%s,%s,%s,%ld\n", 
                    dsBenhNhan[i].maDinhDanh,
                    dsBenhNhan[i].bhyt,
                    dsBenhNhan[i].soDienThoai,
                    dsBenhNhan[i].ten,
                    dsBenhNhan[i].ngaySinh.ngay,
                    dsBenhNhan[i].ngaySinh.thang,
                    dsBenhNhan[i].ngaySinh.nam,
                    dsBenhNhan[i].tuoi,
                    gioiTinhToString(dsBenhNhan[i].gioiTinh),
                    dsBenhNhan[i].diaChi,
                    dsBenhNhan[i].tieuSuBenhLy,
                    (long)dsBenhNhan[i].ngayTaoHoSo);
        }
        fclose(f);
    }
    
    // int taoMaDinhDanhBenhNhanMoi(const BenhNhan dsBenhNhan[], int soLuongBenhNhan) { // <<<< XOA HAM NAY
    //     // ...
    // }
    
    // Ham tim benh nhan theo ma dinh danh (noi bo, de kiem tra trung lap)
    static const BenhNhan* timBenhNhanTheoMaNoiBo(const BenhNhan dsBenhNhan[], int soLuong, int maDD) {
        for (int i = 0; i < soLuong; ++i) {
            if (dsBenhNhan[i].maDinhDanh == maDD) {
                return &dsBenhNhan[i];
            }
        }
        return NULL;
    }

    // Ham tim benh nhan theo ma dinh danh (chuoi) - co the la static neu chi dung noi bo
const BenhNhan* timBenhNhanTheoMaChuoi(const BenhNhan dsBenhNhan[], int soLuong, const char* maDD_str) {
    if (maDD_str == NULL) return NULL;
    for (int i = 0; i < soLuong; ++i) {
        if (strcmp(dsBenhNhan[i].maDinhDanh, maDD_str) == 0) {
            return &dsBenhNhan[i];
        }
    }
    return NULL;
}

int nhapThongTinBenhNhanTuBanPhim(BenhNhan* bn, const BenhNhan dsBenhNhanHienCo[], int soLuongHienCo) {
    char buffer[1000]; 
    char maDD_tam_str[20];
    int lanThu = 0;
    const int MAX_THU_NHAP_MA = 3; // Gioi han so lan nhap sai

    // Nhap va kiem tra Ma Dinh Danh
    do {
        printf("Nhap Ma Dinh Danh Benh Nhan (toi da %d ky tu, VD: 007, BN001): ", 19);
        if (docDongAnToan(maDD_tam_str, sizeof(maDD_tam_str), stdin) == NULL || strlen(maDD_tam_str) == 0) {
            printf("Loi: Ma Dinh Danh khong duoc de trong.\n");
            lanThu++;
            if (lanThu >= MAX_THU_NHAP_MA) { printf("Huy them benh nhan do nhap sai nhieu lan.\n"); return 0;}
            continue;
        }
        // Tuy chon: Chuyen ma sang chu hoa de nhat quan
        // for(int k=0; maDD_tam_str[k]; k++) maDD_tam_str[k] = toupper(maDD_tam_str[k]);

        if (timBenhNhanTheoMaChuoi(dsBenhNhanHienCo, soLuongHienCo, maDD_tam_str) != NULL) {
            printf("Loi: Ma Dinh Danh '%s' da ton tai. Vui long nhap ma khac.\n", maDD_tam_str);
            lanThu++;
            if (lanThu >= MAX_THU_NHAP_MA) { printf("Huy them benh nhan do nhap sai nhieu lan.\n"); return 0;}
            continue;
        }
        strcpy(bn->maDinhDanh, maDD_tam_str); 
        break; 
    } while (lanThu < MAX_THU_NHAP_MA);

    if(lanThu >= MAX_THU_NHAP_MA) { // Neu thoat do het lan thu ma van chua nhap dung
        return 0;
    }

    printf("Nhap BHYT (toi da 15 ky tu, bo trong neu khong co): "); 
    docDongAnToan(bn->bhyt, sizeof(bn->bhyt), stdin);
    printf("Nhap So Dien Thoai (toi da 14 ky tu): "); 
    docDongAnToan(bn->soDienThoai, sizeof(bn->soDienThoai), stdin);
    printf("Nhap Ho va Ten (toi da 99 ky tu): "); 
    docDongAnToan(bn->ten, sizeof(bn->ten), stdin);

    printf("Nhap Ngay Sinh (ngay thang nam, cach nhau boi khoang trang, vd: 15 01 1990): ");
    if (docDongAnToan(buffer, sizeof(buffer), stdin) != NULL &&
        sscanf(buffer, "%d %d %d", &bn->ngaySinh.ngay, &bn->ngaySinh.thang, &bn->ngaySinh.nam) == 3) {
        // Them kiem tra tinh hop le cua ngay thang (neu can)
    } else {
        printf("Dinh dang ngay sinh khong hop le. Dat gia tri mac dinh (1/1/1900).\n");
        bn->ngaySinh.ngay = 1; bn->ngaySinh.thang = 1; bn->ngaySinh.nam = 1900;
    }

    printf("Nhap Tuoi: ");
    if (docDongAnToan(buffer, sizeof(buffer), stdin) != NULL && sscanf(buffer, "%d", &bn->tuoi) == 1 && bn->tuoi >=0) {
        // Hop le
    } else {
        printf("Tuoi khong hop le. Dat gia tri mac dinh 0.\n");
        bn->tuoi = 0;
    }

    printf("Nhap Gioi Tinh (Nam, Nu, Khac): "); 
    docDongAnToan(buffer, sizeof(buffer), stdin);
    bn->gioiTinh = stringToGioiTinh(buffer);

    printf("Nhap Dia Chi (toi da 199 ky tu): "); 
    docDongAnToan(bn->diaChi, sizeof(bn->diaChi), stdin);

    printf("Nhap Tieu Su Benh Ly (neu co, toi da 999 ky tu): "); 
    docDongAnToan(bn->tieuSuBenhLy, sizeof(bn->tieuSuBenhLy), stdin);

    bn->ngayTaoHoSo = time(NULL);
    return 1; // Nhap thanh cong
}

    
    int themBenhNhan(BenhNhan** dsBenhNhanConTro, int* soLuongBenhNhan, BenhNhan benhNhanMoi, const char* tenFile) {
        // Kiem tra trung lap Ma Dinh Danh mot lan nua truoc khi them vao danh sach
        // (mac du da kiem tra trong ham nhap, nhung de an toan hon)
        if (timBenhNhanTheoMaChuoi(*dsBenhNhanConTro, *soLuongBenhNhan, benhNhanMoi.maDinhDanh) != NULL) {
    printf("Loi nghiem trong: Ma Dinh Danh '%s' da ton tai ngay truoc khi them!\n", benhNhanMoi.maDinhDanh);
    return 0; 
        }
        BenhNhan* temp = realloc(*dsBenhNhanConTro, (*soLuongBenhNhan + 1) * sizeof(BenhNhan));
        if (temp == NULL) {
            perror("Loi cap phat bo nho (realloc themBenhNhan)");
            return 0; 
        }
        *dsBenhNhanConTro = temp;
    
        (*dsBenhNhanConTro)[*soLuongBenhNhan] = benhNhanMoi;
        (*soLuongBenhNhan)++;
    
        ghiDanhSachBenhNhanRaFile(tenFile, *dsBenhNhanConTro, *soLuongBenhNhan);
        return 1; 
    }
    
    int suaThongTinBenhNhan(BenhNhan dsBenhNhan[], int soLuongBenhNhan, char* maDinhDanhCanSua, BenhNhan thongTinMoi, const char* tenFile) {
    int timThayIndex = -1;
    for (int i = 0; i < soLuongBenhNhan; ++i) {
        if (strcmp(dsBenhNhan[i].maDinhDanh, maDinhDanhCanSua) == 0) {
            timThayIndex = i;
            break;
        }
    }

    if (timThayIndex != -1) {
        // Ma Dinh Danh khong duoc sua. Sao chep ma cu vao thong tin moi.
        strcpy(thongTinMoi.maDinhDanh, dsBenhNhan[timThayIndex].maDinhDanh); 
        thongTinMoi.ngayTaoHoSo = dsBenhNhan[timThayIndex].ngayTaoHoSo; // Giu nguyen ngay tao ho so
        dsBenhNhan[timThayIndex] = thongTinMoi; // Cap nhat cac truong khac

        ghiDanhSachBenhNhanRaFile(tenFile, dsBenhNhan, soLuongBenhNhan);
        return 1;
    }
    return 0; // Khong tim thay
}

    
    int xoaBenhNhan(BenhNhan** dsBenhNhanConTro, int* soLuongBenhNhan, char* maDinhDanhCanXoa, const char* tenFile) {
        // Ham nay nhan maDinhDanhCanXoa la mot ky tu (char), nen can so sanh voi ky tu dau tien cua maDinhDanh
        int viTriCanXoa = -1;
        for (int i = 0; i < *soLuongBenhNhan; ++i) {
           
            if (strcmp((*dsBenhNhanConTro)[i].maDinhDanh, maDinhDanhCanXoa) == 0) {
                viTriCanXoa = i;
                break;
            }
        
        }    if (viTriCanXoa != -1) {
            for (int i = viTriCanXoa; i < (*soLuongBenhNhan - 1); ++i) {
                (*dsBenhNhanConTro)[i] = (*dsBenhNhanConTro)[i + 1];
            }
            (*soLuongBenhNhan)--;

            if (*soLuongBenhNhan == 0) {
                free(*dsBenhNhanConTro);
                *dsBenhNhanConTro = NULL;
            } else {
                BenhNhan* temp = realloc(*dsBenhNhanConTro, (*soLuongBenhNhan) * sizeof(BenhNhan));
                if (temp == NULL && *soLuongBenhNhan > 0) { 
                    perror("Loi realloc khi xoa benh nhan (thu nho mang)");
                } else if (*soLuongBenhNhan > 0) {
                    *dsBenhNhanConTro = temp;
                } else { // Neu soLuongBenhNhan == 0 sau realloc (hiem khi) hoac temp la NULL
                    free(*dsBenhNhanConTro); // Giai phong con tro cu neu realloc that bai va so luong la 0
                    *dsBenhNhanConTro = NULL;
                }
            }
           ghiDanhSachBenhNhanRaFile(tenFile, *dsBenhNhanConTro, *soLuongBenhNhan);
            return 1; 
        }
        return 0; 
    }
    
    void inDanhSachBenhNhanConsole(const BenhNhan dsBenhNhan[], int soLuongBenhNhan) {
        printf("\n--- DANH SACH BENH NHAN ---\n");
        if (soLuongBenhNhan == 0) {
            printf("Khong co benh nhan nao trong danh sach.\n");
            return;
        }

        // Dinh dang cho printf, su dung * de truyen do rong cot mot cach dong
        printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("| %-5s | %-18s | %-15s | %-15s | %-*s | %-10s | %-5s | %-8s | %-*s | %-*s |\n",
            "STT", "Ma DD", "BHYT", "SDT",
            PATIENT_HO_TEN_COL_WIDTH, "Ho Ten",        // Su dung define
            "Ngay Sinh", "Tuoi", "GioiTinh",
            PATIENT_DIA_CHI_COL_WIDTH, "Dia Chi",      // Su dung define
            PATIENT_TIEU_SU_COL_WIDTH, "Tieu Su Benh Ly"); // Su dung define
        printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

        for (int i = 0; i < soLuongBenhNhan; ++i) {
            char ngaySinhStr[11];
            snprintf(ngaySinhStr, sizeof(ngaySinhStr), "%02d/%02d/%04d",
                dsBenhNhan[i].ngaySinh.ngay, dsBenhNhan[i].ngaySinh.thang, dsBenhNhan[i].ngaySinh.nam);

            // Buffer phai du lon de chua chuoi da rut gon (VD: doRong + 1 cho '\0')
            // >>> SUA: Su dung #define de khai bao kich thuoc mang <<<
            char hoTenDisplay[PATIENT_HO_TEN_COL_WIDTH + 1];
            rutGonChuoiHienThi(hoTenDisplay, sizeof(hoTenDisplay), dsBenhNhan[i].ten, PATIENT_HO_TEN_COL_WIDTH);

            char diaChiDisplay[PATIENT_DIA_CHI_COL_WIDTH + 1];
            rutGonChuoiHienThi(diaChiDisplay, sizeof(diaChiDisplay), dsBenhNhan[i].diaChi, PATIENT_DIA_CHI_COL_WIDTH);

            char tieuSuDisplay[PATIENT_TIEU_SU_COL_WIDTH + 1];
            rutGonChuoiHienThi(tieuSuDisplay, sizeof(tieuSuDisplay), dsBenhNhan[i].tieuSuBenhLy, PATIENT_TIEU_SU_COL_WIDTH);

            printf("| %-5d | %-18s | %-15s | %-15s | %-*s | %-10s | %-5d | %-8s | %-*s | %-*s |\n",
                i + 1,
                dsBenhNhan[i].maDinhDanh,
                dsBenhNhan[i].bhyt,
                dsBenhNhan[i].soDienThoai,
                PATIENT_HO_TEN_COL_WIDTH, hoTenDisplay, // Truyen define cho do rong
                ngaySinhStr,
                dsBenhNhan[i].tuoi,
                gioiTinhToString(dsBenhNhan[i].gioiTinh),
                PATIENT_DIA_CHI_COL_WIDTH, diaChiDisplay,    // Truyen define cho do rong
                PATIENT_TIEU_SU_COL_WIDTH, tieuSuDisplay); // Truyen define cho do rong
        }
        printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    }
    void timKiemBenhNhan(const BenhNhan dsBenhNhan[], int soLuongBenhNhan,
                         const char* tieuChi, const char* giaTriTimKiem,
                         BenhNhan** ketQuaTimKiem, int* soLuongKetQua) {
        // ... (Ham nay giu nguyen)
        // ... (Copy lai toan bo ham timKiemBenhNhan tu phien ban truoc)
        *ketQuaTimKiem = NULL;
        *soLuongKetQua = 0;
        if (soLuongBenhNhan == 0 || giaTriTimKiem == NULL || tieuChi == NULL) {
            return;
        }

        BenhNhan* dsKetQuaTamThoi = malloc(soLuongBenhNhan * sizeof(BenhNhan)); 
        if (dsKetQuaTamThoi == NULL) {
            perror("Loi cap phat bo nho (timKiemBenhNhan)");
            return;
        }
        int count = 0;

        char giaTriTimKiemLower[100];
        strncpy(giaTriTimKiemLower, giaTriTimKiem, sizeof(giaTriTimKiemLower)-1);
        giaTriTimKiemLower[sizeof(giaTriTimKiemLower)-1] = '\0';
        for(int i = 0; giaTriTimKiemLower[i]; i++) giaTriTimKiemLower[i] = tolower(giaTriTimKiemLower[i]);

        char tieuChiLower[50];
        strncpy(tieuChiLower, tieuChi, sizeof(tieuChiLower)-1);
        tieuChiLower[sizeof(tieuChiLower)-1] = '\0';
        for(int i = 0; tieuChiLower[i]; i++) tieuChiLower[i] = tolower(tieuChiLower[i]);


        for (int i = 0; i < soLuongBenhNhan; ++i) {
            int timThayTrongVongLapNay = 0;
            if (strcmp(tieuChiLower, "madinhdanh") == 0) {
                if (strcmp(dsBenhNhan[i].maDinhDanh, giaTriTimKiem) == 0) { // atoi van an toan neu giaTriTimKiem khong phai so
                    timThayTrongVongLapNay = 1;
                }
            } else if (strcmp(tieuChiLower, "bhyt") == 0) {
                if (strcmp(dsBenhNhan[i].bhyt, giaTriTimKiem) == 0) {
                    timThayTrongVongLapNay = 1;
                }
            } else if (strcmp(tieuChiLower, "sodienthoai") == 0) {
                if (strcmp(dsBenhNhan[i].soDienThoai, giaTriTimKiem) == 0) {
                    timThayTrongVongLapNay = 1;
                }
            } else if (strcmp(tieuChiLower, "ten") == 0) {
                char tenBenhNhanLower[100];
                strncpy(tenBenhNhanLower, dsBenhNhan[i].ten, sizeof(tenBenhNhanLower)-1);
                tenBenhNhanLower[sizeof(tenBenhNhanLower)-1] = '\0';
                for(int k=0; tenBenhNhanLower[k]; k++) tenBenhNhanLower[k] = tolower(tenBenhNhanLower[k]);

                if (strstr(tenBenhNhanLower, giaTriTimKiemLower) != NULL) {
                    timThayTrongVongLapNay = 1;
                }
            }

            if (timThayTrongVongLapNay) {
                dsKetQuaTamThoi[count++] = dsBenhNhan[i];
            }
        }

        if (count > 0) {
            *ketQuaTimKiem = malloc(count * sizeof(BenhNhan));
            if (*ketQuaTimKiem == NULL) {
                perror("Loi cap phat bo nho cho ket qua tim kiem");
                free(dsKetQuaTamThoi);
                return;
            }
            memcpy(*ketQuaTimKiem, dsKetQuaTamThoi, count * sizeof(BenhNhan));
            *soLuongKetQua = count;
        }
        free(dsKetQuaTamThoi);
    }
    