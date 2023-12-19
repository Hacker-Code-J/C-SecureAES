fn rot_word(word: u32) -> u32 {
    (word << 8) | (word >> 24)
}

fn sub_word(word: u32, s_box: &[u8; 256]) -> u32 {
    ((s_box[(word >> 24) as usize] as u32) << 24) |
    ((s_box[((word >> 16) & 0xff) as usize] as u32) << 16) |
    ((s_box[((word >> 8) & 0xff) as usize] as u32) << 8) |
    (s_box[(word & 0xff) as usize] as u32)
}

fn key_expansion(u_key: &[u8], r_key: &mut [u32], nk: usize, nr: usize, r_con: &[u32], s_box: &[u8; 256]) {
    for i in 0..nk {
        r_key[i] = ((u_key[4*i] as u32) << 24) |
                   ((u_key[4*i + 1] as u32) << 16) |
                   ((u_key[4*i + 2] as u32) << 8) |
                   (u_key[4*i + 3] as u32);
    }

    for i in nk..((nr + 1) * 4) {
        let mut temp = r_key[i - 1];
        if i % nk == 0 {
            temp = sub_word(rot_word(temp), s_box) ^ r_con[i / nk - 1];
        } else if nk > 6 && i % nk == 4 {
            temp = sub_word(temp, s_box);
        }
        r_key[i] = r_key[i - nk] ^ temp;
    }
}
