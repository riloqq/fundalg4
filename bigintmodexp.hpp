BigInt BigInt::mod_exp(const BigInt& base, const BigInt& exp, const BigInt& mod) {
    if (exp.is_empty()) {
        return BigInt(1, exp.base);
    }

    if (mod == BigInt(1)) {
        return BigInt(0);
    }

    BigInt result(1, base.base);
    BigInt current_base = base % mod;
    BigInt current_exp = exp;
    BigInt two(2);

    while (current_exp > BigInt(0)) {
        if ((current_exp.digits[0] % 2) != 0) {
            result = (result * current_base) % mod;
        }
        current_base = (current_base * current_base) % mod;
        current_exp = current_exp / two;
    }

    return result;
}
