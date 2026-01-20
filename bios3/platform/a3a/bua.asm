;;
;; Copyright (c) 2026, Ian Moffett.
;; Provided under the BSD-3 clause.
;;

    .org 0x0000
    .section .text
_start:
    ;;
    ;; Represents the bring-up area, firmware execution begins
    ;; here.
    ;;

    ;;
    ;; The processor stores its interrupt flags in special
    ;; register 1, read that and set bit 1 [interrupt mask]
    ;;
    mov g14, 0x0001
    mov g15, 0x0000
    srr
    or g0, 1
    srw
hang:
    hlt
    b hang
