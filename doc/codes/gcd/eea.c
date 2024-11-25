u32 eea(u32 a, u32 b, u32* x, u32* y) {
    u32 an, bn; u32 ua, va, ub, vb; u32 new_an, new_bn;
    u32 n_ua, n_va, n_ub, n_vb; u32 q;

    an = a; bn = b;
    ua = 1; va = 0; ub = 0; vb = 1;
    while (bn != 0) {
        q = an / bn;
        new_an = bn;
        new_bn = an - bn * q;

        n_ua = ub; n_va = vb;
        n_ub = ua - ub * q; n_vb = va - vb * q;
        
        an = new_an; bn = new_bn;
        ua = n_ua; va = n_va; ub = n_ub; vb = n_vb;
    }
    *x = ua; *y = va;
    return an;
}
