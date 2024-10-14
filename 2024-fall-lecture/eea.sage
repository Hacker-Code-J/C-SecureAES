# Define the finite field GF(2)
F = GF(2)

# Define the polynomial ring over GF(2)
R.<x> = PolynomialRing(F)

def extended_euclidean_algorithm_poly(a, b):
    """
    Perform the Extended Euclidean Algorithm (EEA) on polynomials a and b over GF(2),
    and print the step-by-step process.
    
    Returns:
        gcd(a, b), s(x), t(x) such that gcd(a, b) = a * s(x) + b * t(x)
    """
    
    # Initial setup
    s0, s1 = R(1), R(0)
    t0, t1 = R(0), R(1)
    
    # Print initial values
    print(f"Initial polynomials:")
    print(f"a(x) = {a}")
    print(f"b(x) = {b}")
    print(f"\nStart of EEA process:")
    
    while b != 0:
        # Compute quotient and remainder
        q, r = a.quo_rem(b)
        
        # Print current step
        print(f"\nq(x) = {q}, r(x) = {r}")
        print(f"a(x) = [{a}] = [{q}] * [{b}] + [{r}]")
        
        # Update a, b
        a, b = b, r
    
        print(f"s1 = {s0} - {q} * {s1}")
        print(f"t1 = {t0} - {q} * {t1}")

        # Update s and t
        s0, s1 = s1, s0 - q * s1
        t0, t1 = t1, t0 - q * t1

        # Print the update for s and t
        print(f"s(x) = {s1}, t(x) = {t1}")
    
    # At the end of the loop, a is the gcd, and s0, t0 are the Bézout coefficients
    print("\nEEA complete.")
    print(f"gcd(a(x), b(x)) = {a}")
    print(f"Bézout coefficients: s(x) = {s0}, t(x) = {t0}")
    
    return a, s0, t0

# Example usage:
a = x^8 + x^4 + x^3 + x + 1  # Define a polynomial a(x) in GF(2)[x]
b = x^7 + x^6 + x^5 + x^4 + 1           # Define a polynomial b(x) in GF(2)[x]

gcd, s, t = extended_euclidean_algorithm_poly(a, b)
