#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "structs.h" // Can LanKham, MucDoUuTien, BenhNhan (de in ten)
#include <stdio.h>    // Cho NULL

// Cau truc du lieu cho Hang Doi Uu Tien
// Se su dung mot mang dong cac LanKham, duoc sap xep.
typedef struct {
    LanKham* dsChoKham; // Mang dong luu cac LanKham dang cho
    int soLuongHienTai; // So luong LanKham hien co trong hang doi
    int dungLuong;      // Dung luong toi da hien tai cua mang dsChoKham
} HangDoiUuTien;

// --- Khai bao cac ham ---

/**
 * @brief Khoi tao mot hang doi uu tien moi.
 *
 * @param hq Con tro toi HangDoiUuTien can khoi tao.
 */
void khoiTaoHangDoi(HangDoiUuTien* hq);

/**
 * @brief Them mot LanKham (phai o trang thai LK_DANG_CHO) vao hang doi.
 * Hang doi se duoc sap xep lai sau khi them.
 *
 * @param hq Con tro toi HangDoiUuTien.
 * @param lanKhamMoi LanKham can them vao hang doi.
 * @return 1 neu them thanh cong, 0 neu that bai (vi du: het bo nho).
 */
int themVaoHangDoi(HangDoiUuTien* hq, LanKham lanKhamMoi);

/**
 * @brief Lay va xoa LanKham co uu tien cao nhat (dau hang doi) khoi hang doi.
 *
 * @param hq Con tro toi HangDoiUuTien.
 * @param lanKhamLayRa Con tro de luu tru LanKham duoc lay ra.
 * @return 1 neu lay thanh cong, 0 neu hang doi rong.
 */
int layBenhNhanTiepTheo(HangDoiUuTien* hq, LanKham* lanKhamLayRa);

/**
 * @brief Kiem tra xem hang doi co rong hay khong.
 *
 * @param hq Con tro toi HangDoiUuTien.
 * @return 1 neu rong, 0 neu khong rong.
 */
int laHangDoiRong(const HangDoiUuTien* hq);

/**
 * @brief In ra danh sach benh nhan hien dang cho trong hang doi, theo thu tu uu tien.
 *
 * @param hq Con tro toi HangDoiUuTien.
 * @param dsBenhNhanToanBo Mang tat ca benh nhan (de lay ten).
 * @param soLuongBNTB So luong benh nhan trong mang dsBenhNhanToanBo.
 */
void inHangDoiHienTaiConsole(const HangDoiUuTien* hq, const BenhNhan dsBenhNhanToanBo[], int soLuongBNTB);

/**
 * @brief Giai phong bo nho da cap phat cho hang doi.
 *
 * @param hq Con tro toi HangDoiUuTien.
 */
void giaiPhongHangDoi(HangDoiUuTien* hq);

/**
 * @brief (Ham noi bo) Sap xep lai hang doi theo muc do uu tien va thoi gian dang ky.
 * Su dung mot thuat toan sap xep don gian tu cai dat (vi du: Insertion Sort).
 * MucDoUuTien cang nho cang uu tien. Neu bang nhau, gioDangKyThanhCong cang nho cang uu tien.
 *
 * @param hq Con tro toi HangDoiUuTien.
 */
// static void sapXepHangDoi(HangDoiUuTien* hq); // Se duoc goi noi bo


/**
 * @brief (Ham noi bo) So sanh hai LanKham de sap xep hang doi.
 * @return -1 neu lk1 < lk2 (uu tien hon)
 * @return  1 neu lk1 > lk2 (kem uu tien hon)
 * @return  0 neu lk1 == lk2 (uu tien nhu nhau - hiem khi xay ra neu thoi gian khac nhau)
 */
// static int soSanhUuTienLanKham(const LanKham* lk1, const LanKham* lk2); // Se duoc goi noi bo

#endif // PRIORITY_QUEUE_H