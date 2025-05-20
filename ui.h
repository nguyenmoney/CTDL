#ifndef UI_H
#define UI_H

#include "structs.h"
#include "patient.h"
#include "examination.h"
#include "department.h"
#include "doctor.h"
#include "medicine.h"
#include "prescription.h"
#include "priority_queue.h"
#include "csv_handler.h" // Cho docDongAnToan

// --- Khai bao cac ham giao dien ---

/**
 * @brief Hien thi menu chinh cua chuong trinh.
 */
void hienThiMenuChinh();

/**
 * @brief Xu ly lua chon cua nguoi dung tu menu chinh.
 * Ham nay se dieu phoi viec goi cac ham chuc nang tuong ung.
 * Tat ca cac con tro toi mang du lieu va so luong deu la con tro cap hai
 * de cac ham trong UI co the thay doi truc tiep du lieu goc (vi du: sau khi them/xoa).
 */
// File: include/ui.h - SAU KHI SUA
// ...
void xuLyLuaChonMenuChinh(
    int choice, // <<<< THEM THAM SO NAY
    BenhNhan** dsBenhNhan, int* soLuongBenhNhan, const char* fileBN,
    LanKham** dsLanKham, int* soLuongLanKham, const char* fileLK,
    KhoaPhong** dsKhoaPhong, int* soLuongKhoaPhong, const char* fileKP,
    BacSi** dsBacSi, int* soLuongBacSi, const char* fileBS,
    Thuoc** dsThuoc, int* soLuongThuoc, const char* fileT,
    DonThuocChiTiet** dsDonThuocChiTiet, int* soLuongDTCT, const char* fileDTCT,
    HangDoiUuTien* hangDoiKhamBenh
);
// ...


// --- Cac ham menu con ---
void menuQuanLyBenhNhan(BenhNhan** dsBenhNhan, int* soLuongBenhNhan, const char* tenFileCSV);
void xuLyLuaChonMenuChinh(
    int choice, // <<<< THEM THAM SO NAY 
    BenhNhan** dsBenhNhan, int* soLuongBenhNhan, const char* fileBN,
    LanKham** dsLanKham, int* soLuongLanKham, const char* fileLK,
    KhoaPhong** dsKhoaPhong, int* soLuongKhoaPhong, const char* fileKP,
    BacSi** dsBacSi, int* soLuongBacSi, const char* fileBS,        
    Thuoc** dsThuoc, int* soLuongThuoc, const char* fileT,
    DonThuocChiTiet** dsDonThuocChiTiet, int* soLuongDTCT, const char* fileDTCT,
    HangDoiUuTien* hangDoiKhamBenh);
void menuQuanLyLanKham(
    LanKham** dsLanKham, int* soLuongLanKham,     
    BenhNhan* dsBenhNhan, int soLuongBenhNhan,    
    BacSi* dsBacSi, int soLuongBacSi,            
    KhoaPhong* dsKhoaPhong, int soLuongKhoaPhong, 
    const char* fileLanKhamCSV);
void menuQuanLyBacSi(BacSi** dsBacSi, int* soLuongBacSi, const char* tenFileCSV, KhoaPhong* dsKhoaPhong, int soLuongKhoaPhong);
void menuQuanLyThuoc(Thuoc** dsThuoc, int* soLuongThuoc, const char* tenFileCSV);


// --- Cac ham tien ich giao dien ---

/**
 * @brief Xoa man hinh console (tuong thich da nen tang o muc co ban).
 */
void xoaManHinhConsole();

/**
 * @brief Dung man hinh, cho nguoi dung nhan Enter de tiep tuc.
 */
void dungManHinhCho();

/**
 * @brief Nhap mot so nguyen tu nguoi dung, co kiem tra gioi han.
 * Se yeu cau nhap lai neu khong hop le.
 *
 * @param prompt Thong bao hien thi cho nguoi dung.
 * @param min Gia tri toi thieu cho phep.
 * @param max Gia tri toi da cho phep.
 * @return So nguyen hop le do nguoi dung nhap.
 */
int nhapSoNguyenCoGioiHan(const char* prompt, int min, int max);

/**
 * @brief Nhap mot so nguyen khong am tu nguoi dung.
 * @param prompt Thong bao hien thi.
 * @return So nguyen khong am, hoac -1 neu nhap khong hop le.
 */
int nhapSoNguyenKhongAm(const char* prompt);

/**
 * @brief In mot thong diep loi.
 * @param message Thong diep loi can in.
 */
void inThongDiepLoi(const char* message);

/**
 * @brief In mot thong diep thanh cong.
 * @param message Thong diep thanh cong can in.
 */
void inThongDiepThanhCong(const char* message);


#endif // UI_H