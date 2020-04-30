#include "eigenvalues.hh"

#include <cmath> // std::hypot

// This file includes an adapted version of the Eigendecomposition class from Jama (https://math.nist.gov/javanumerics/jama/)
// which is in public domain.

namespace tg
{
namespace detail
{
template <int D, class ScalarT>
class eigenvalue_decomposition_impl
{
private:
    /* ------------------------
       Class variables
     * ------------------------ */

    /** Row and column dimension (square matrix).
    @serial matrix dimension.
    */
    //    int D;

    /** Symmetry flag.
    @serial internal symmetry flag.
    */
    bool issymmetric;

    /** Arrays for internal storage of eigenvalues.
    @serial internal storage of eigenvalues.
    */
    array<ScalarT, D> d;
    array<ScalarT, D> e;

    /** Array for internal storage of eigenvectors.
    @serial internal storage of eigenvectors.
    */
    array<array<ScalarT, D>, D> V;

    /** Array for internal storage of nonsymmetric Hessenberg form.
    @serial internal storage of nonsymmetric Hessenberg form.
    */
    array<array<ScalarT, D>, D> H;

    /** Working storage for nonsymmetric algorithm.
    @serial working storage for nonsymmetric algorithm.
    */
    array<ScalarT, D> ort;

    /* ------------------------
       Private Methods
     * ------------------------ */

    // Symmetric Householder reduction to tridiagonal form.

    void tred2()
    {
        //  This is derived from the Algol procedures tred2 by
        //  Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
        //  Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.

        for (int j = 0; j < D; j++)
        {
            d[j] = V[D - 1][j];
        }

        // Householder reduction to tridiagonal form.

        for (int i = D - 1; i > 0; i--)
        {
            // Scale to avoid under/overflow.

            ScalarT scale = ScalarT(0.0);
            ScalarT h = ScalarT(0.0);
            for (int k = 0; k < i; k++)
            {
                scale = scale + tg::abs(d[k]);
            }
            if (scale == ScalarT(0.0))
            {
                e[i] = d[i - 1];
                for (int j = 0; j < i; j++)
                {
                    d[j] = V[i - 1][j];
                    V[i][j] = ScalarT(0.0);
                    V[j][i] = ScalarT(0.0);
                }
            }
            else
            {
                // Generate Householder vector.

                for (int k = 0; k < i; k++)
                {
                    d[k] /= scale;
                    h += d[k] * d[k];
                }
                ScalarT f = d[i - 1];
                ScalarT g = tg::sqrt(h);
                if (f > 0)
                {
                    g = -g;
                }
                e[i] = scale * g;
                h = h - f * g;
                d[i - 1] = f - g;
                for (int j = 0; j < i; j++)
                {
                    e[j] = ScalarT(0.0);
                }

                // Apply similarity transformation to remaining columns.

                for (int j = 0; j < i; j++)
                {
                    f = d[j];
                    V[j][i] = f;
                    g = e[j] + V[j][j] * f;
                    for (int k = j + 1; k <= i - 1; k++)
                    {
                        g += V[k][j] * d[k];
                        e[k] += V[k][j] * f;
                    }
                    e[j] = g;
                }
                f = ScalarT(0.0);
                for (int j = 0; j < i; j++)
                {
                    e[j] /= h;
                    f += e[j] * d[j];
                }
                ScalarT hh = f / (h + h);
                for (int j = 0; j < i; j++)
                {
                    e[j] -= hh * d[j];
                }
                for (int j = 0; j < i; j++)
                {
                    f = d[j];
                    g = e[j];
                    for (int k = j; k <= i - 1; k++)
                    {
                        V[k][j] -= (f * e[k] + g * d[k]);
                    }
                    d[j] = V[i - 1][j];
                    V[i][j] = ScalarT(0.0);
                }
            }
            d[i] = h;
        }

        // Accumulate transformations.

        for (int i = 0; i < D - 1; i++)
        {
            V[D - 1][i] = V[i][i];
            V[i][i] = ScalarT(1.0);
            ScalarT h = d[i + 1];
            if (h != ScalarT(0.0))
            {
                for (int k = 0; k <= i; k++)
                {
                    d[k] = V[k][i + 1] / h;
                }
                for (int j = 0; j <= i; j++)
                {
                    ScalarT g = ScalarT(0.0);
                    for (int k = 0; k <= i; k++)
                    {
                        g += V[k][i + 1] * V[k][j];
                    }
                    for (int k = 0; k <= i; k++)
                    {
                        V[k][j] -= g * d[k];
                    }
                }
            }
            for (int k = 0; k <= i; k++)
            {
                V[k][i + 1] = ScalarT(0.0);
            }
        }
        for (int j = 0; j < D; j++)
        {
            d[j] = V[D - 1][j];
            V[D - 1][j] = ScalarT(0.0);
        }
        V[D - 1][D - 1] = ScalarT(1.0);
        e[0] = ScalarT(0.0);
    }

    // Symmetric tridiagonal QL algorithm.

    void tql2()
    {
        //  This is derived from the Algol procedures tql2, by
        //  Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
        //  Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.

        for (int i = 1; i < D; i++)
        {
            e[i - 1] = e[i];
        }
        e[D - 1] = ScalarT(0.0);

        ScalarT f = ScalarT(0.0);
        ScalarT tst1 = ScalarT(0.0);
        ScalarT eps = tg::pow(ScalarT(2.0), ScalarT(-52.0));
        for (int l = 0; l < D; l++)
        {
            // Find small subdiagonal element

            tst1 = tg::max(tst1, tg::abs(d[l]) + tg::abs(e[l]));
            int m = l;
            while (m < D)
            {
                if (tg::abs(e[m]) <= eps * tst1)
                {
                    break;
                }
                m++;
            }

            // If m == l, d[l] is an eigenvalue,
            // otherwise, iterate.

            if (m > l)
            {
                int iter = 0;
                do
                {
                    iter = iter + 1; // (Could check iteration count here.)

                    // Compute implicit shift

                    ScalarT g = d[l];
                    ScalarT p = (d[l + 1] - g) / (ScalarT(2.0) * e[l]);
                    ScalarT r = std::hypot(p, ScalarT(1.0));
                    if (p < 0)
                    {
                        r = -r;
                    }
                    d[l] = e[l] / (p + r);
                    d[l + 1] = e[l] * (p + r);
                    ScalarT dl1 = d[l + 1];
                    ScalarT h = g - d[l];
                    for (int i = l + 2; i < D; i++)
                    {
                        d[i] -= h;
                    }
                    f = f + h;

                    // Implicit QL transformation.

                    p = d[m];
                    ScalarT c = ScalarT(1.0);
                    ScalarT c2 = c;
                    ScalarT c3 = c;
                    ScalarT el1 = e[l + 1];
                    ScalarT s = ScalarT(0.0);
                    ScalarT s2 = ScalarT(0.0);
                    for (int i = m - 1; i >= l; i--)
                    {
                        c3 = c2;
                        c2 = c;
                        s2 = s;
                        g = c * e[i];
                        h = c * p;
                        r = std::hypot(p, e[i]);
                        e[i + 1] = s * r;
                        s = e[i] / r;
                        c = p / r;
                        p = c * d[i] - s * g;
                        d[i + 1] = h + s * (c * g + s * d[i]);

                        // Accumulate transformation.

                        for (int k = 0; k < D; k++)
                        {
                            h = V[k][i + 1];
                            V[k][i + 1] = s * V[k][i] + c * h;
                            V[k][i] = c * V[k][i] - s * h;
                        }
                    }
                    p = -s * s2 * c3 * el1 * e[l] / dl1;
                    e[l] = s * p;
                    d[l] = c * p;

                    // Check for convergence.

                } while (tg::abs(e[l]) > eps * tst1);
            }
            d[l] = d[l] + f;
            e[l] = ScalarT(0.0);
        }

        // Sort eigenvalues and corresponding vectors.

        for (int i = 0; i < D - 1; i++)
        {
            int k = i;
            ScalarT p = d[i];
            for (int j = i + 1; j < D; j++)
            {
                if (d[j] < p)
                {
                    k = j;
                    p = d[j];
                }
            }
            if (k != i)
            {
                d[k] = d[i];
                d[i] = p;
                for (int j = 0; j < D; j++)
                {
                    p = V[j][i];
                    V[j][i] = V[j][k];
                    V[j][k] = p;
                }
            }
        }
    }

    // Nonsymmetric reduction to Hessenberg form.

    void orthes()
    {
        //  This is derived from the Algol procedures orthes and ortran,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutines in EISPACK.

        int low = 0;
        int high = D - 1;

        for (int m = low + 1; m <= high - 1; m++)
        {
            // Scale column.

            ScalarT scale = ScalarT(0.0);
            for (int i = m; i <= high; i++)
            {
                scale = scale + tg::abs(H[i][m - 1]);
            }
            if (scale != ScalarT(0.0))
            {
                // Compute Householder transformation.

                ScalarT h = ScalarT(0.0);
                for (int i = high; i >= m; i--)
                {
                    ort[i] = H[i][m - 1] / scale;
                    h += ort[i] * ort[i];
                }
                ScalarT g = tg::sqrt(h);
                if (ort[m] > 0)
                {
                    g = -g;
                }
                h = h - ort[m] * g;
                ort[m] = ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                for (int j = m; j < D; j++)
                {
                    ScalarT f = ScalarT(0.0);
                    for (int i = high; i >= m; i--)
                    {
                        f += ort[i] * H[i][j];
                    }
                    f = f / h;
                    for (int i = m; i <= high; i++)
                    {
                        H[i][j] -= f * ort[i];
                    }
                }

                for (int i = 0; i <= high; i++)
                {
                    ScalarT f = ScalarT(0.0);
                    for (int j = high; j >= m; j--)
                    {
                        f += ort[j] * H[i][j];
                    }
                    f = f / h;
                    for (int j = m; j <= high; j++)
                    {
                        H[i][j] -= f * ort[j];
                    }
                }
                ort[m] = scale * ort[m];
                H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        for (int i = 0; i < D; i++)
        {
            for (int j = 0; j < D; j++)
            {
                V[i][j] = (i == j ? ScalarT(1.0) : ScalarT(0.0));
            }
        }

        for (int m = high - 1; m >= low + 1; m--)
        {
            if (H[m][m - 1] != ScalarT(0.0))
            {
                for (int i = m + 1; i <= high; i++)
                {
                    ort[i] = H[i][m - 1];
                }
                for (int j = m; j <= high; j++)
                {
                    ScalarT g = ScalarT(0.0);
                    for (int i = m; i <= high; i++)
                    {
                        g += ort[i] * V[i][j];
                    }
                    // ScalarT division avoids possible underflow
                    g = (g / ort[m]) / H[m][m - 1];
                    for (int i = m; i <= high; i++)
                    {
                        V[i][j] += g * ort[i];
                    }
                }
            }
        }
    }


    // Complex scalar division.
    ScalarT cdivr;
    ScalarT cdivi;

    void cdiv(ScalarT xr, ScalarT xi, ScalarT yr, ScalarT yi)
    {
        ScalarT r, d;
        if (tg::abs(yr) > tg::abs(yi))
        {
            r = yi / yr;
            d = yr + r * yi;
            cdivr = (xr + r * xi) / d;
            cdivi = (xi - r * xr) / d;
        }
        else
        {
            r = yr / yi;
            d = yi + r * yr;
            cdivr = (r * xr + xi) / d;
            cdivi = (r * xi - xr) / d;
        }
    }

    // Nonsymmetric reduction from Hessenberg to real Schur form.
    void hqr2()
    {
        //  This is derived from the Algol procedure hqr2,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.

        // Initialize

        int nn = D;
        int n = nn - 1;
        int low = 0;
        int high = nn - 1;
        ScalarT eps = tg::pow(ScalarT(2.0), ScalarT(-52.0));
        ScalarT exshift = ScalarT(0.0);
        ScalarT p = 0, q = 0, r = 0, s = 0, z = 0, t, w, x, y;

        // Store roots isolated by balanc and compute matrix norm

        ScalarT norm = ScalarT(0.0);
        for (int i = 0; i < nn; i++)
        {
            if ((i < low) | (i > high))
            {
                d[i] = H[i][i];
                e[i] = ScalarT(0.0);
            }
            for (int j = tg::max(i - 1, 0); j < nn; j++)
            {
                norm = norm + tg::abs(H[i][j]);
            }
        }

        // Outer loop over eigenvalue index

        int iter = 0;
        while (n >= low)
        {
            // Look for single small sub-diagonal element

            int l = n;
            while (l > low)
            {
                s = tg::abs(H[l - 1][l - 1]) + tg::abs(H[l][l]);
                if (s == ScalarT(0.0))
                {
                    s = norm;
                }
                if (tg::abs(H[l][l - 1]) < eps * s)
                {
                    break;
                }
                l--;
            }

            // Check for convergence
            // One root found

            if (l == n)
            {
                H[n][n] = H[n][n] + exshift;
                d[n] = H[n][n];
                e[n] = ScalarT(0.0);
                n--;
                iter = 0;

                // Two roots found
            }
            else if (l == n - 1)
            {
                w = H[n][n - 1] * H[n - 1][n];
                p = (H[n - 1][n - 1] - H[n][n]) / ScalarT(2.0);
                q = p * p + w;
                z = tg::sqrt(tg::abs(q));
                H[n][n] = H[n][n] + exshift;
                H[n - 1][n - 1] = H[n - 1][n - 1] + exshift;
                x = H[n][n];

                // Real pair

                if (q >= 0)
                {
                    if (p >= 0)
                    {
                        z = p + z;
                    }
                    else
                    {
                        z = p - z;
                    }
                    d[n - 1] = x + z;
                    d[n] = d[n - 1];
                    if (z != ScalarT(0.0))
                    {
                        d[n] = x - w / z;
                    }
                    e[n - 1] = ScalarT(0.0);
                    e[n] = ScalarT(0.0);
                    x = H[n][n - 1];
                    s = tg::abs(x) + tg::abs(z);
                    p = x / s;
                    q = z / s;
                    r = tg::sqrt(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    for (int j = n - 1; j < nn; j++)
                    {
                        z = H[n - 1][j];
                        H[n - 1][j] = q * z + p * H[n][j];
                        H[n][j] = q * H[n][j] - p * z;
                    }

                    // Column modification

                    for (int i = 0; i <= n; i++)
                    {
                        z = H[i][n - 1];
                        H[i][n - 1] = q * z + p * H[i][n];
                        H[i][n] = q * H[i][n] - p * z;
                    }

                    // Accumulate transformations

                    for (int i = low; i <= high; i++)
                    {
                        z = V[i][n - 1];
                        V[i][n - 1] = q * z + p * V[i][n];
                        V[i][n] = q * V[i][n] - p * z;
                    }

                    // Complex pair
                }
                else
                {
                    d[n - 1] = x + p;
                    d[n] = x + p;
                    e[n - 1] = z;
                    e[n] = -z;
                }
                n = n - 2;
                iter = 0;

                // No convergence yet
            }
            else
            {
                // Form shift

                x = H[n][n];
                y = ScalarT(0.0);
                w = ScalarT(0.0);
                if (l < n)
                {
                    y = H[n - 1][n - 1];
                    w = H[n][n - 1] * H[n - 1][n];
                }

                // Wilkinson's original ad hoc shift

                if (iter == 10)
                {
                    exshift += x;
                    for (int i = low; i <= n; i++)
                    {
                        H[i][i] -= x;
                    }
                    s = tg::abs(H[n][n - 1]) + tg::abs(H[n - 1][n - 2]);
                    x = y = ScalarT(0.75) * s;
                    w = ScalarT(-0.4375) * s * s;
                }

                // MATLAB's new ad hoc shift

                if (iter == 30)
                {
                    s = (y - x) / ScalarT(2.0);
                    s = s * s + w;
                    if (s > 0)
                    {
                        s = tg::sqrt(s);
                        if (y < x)
                        {
                            s = -s;
                        }
                        s = x - w / ((y - x) / ScalarT(2.0) + s);
                        for (int i = low; i <= n; i++)
                        {
                            H[i][i] -= s;
                        }
                        exshift += s;
                        x = y = w = ScalarT(0.964);
                    }
                }

                iter = iter + 1; // (Could check iteration count here.)

                // Look for two consecutive small sub-diagonal elements

                int m = n - 2;
                while (m >= l)
                {
                    z = H[m][m];
                    r = x - z;
                    s = y - z;
                    p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
                    q = H[m + 1][m + 1] - z - r - s;
                    r = H[m + 2][m + 1];
                    s = tg::abs(p) + tg::abs(q) + tg::abs(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    if (m == l)
                    {
                        break;
                    }
                    if (tg::abs(H[m][m - 1]) * (tg::abs(q) + tg::abs(r))
                        < eps * (tg::abs(p) * (tg::abs(H[m - 1][m - 1]) + tg::abs(z) + tg::abs(H[m + 1][m + 1]))))
                    {
                        break;
                    }
                    m--;
                }

                for (int i = m + 2; i <= n; i++)
                {
                    H[i][i - 2] = ScalarT(0.0);
                    if (i > m + 2)
                    {
                        H[i][i - 3] = ScalarT(0.0);
                    }
                }

                // ScalarT QR step involving rows l:n and columns m:n


                for (int k = m; k <= n - 1; k++)
                {
                    bool notlast = (k != n - 1);
                    if (k != m)
                    {
                        p = H[k][k - 1];
                        q = H[k + 1][k - 1];
                        r = (notlast ? H[k + 2][k - 1] : ScalarT(0.0));
                        x = tg::abs(p) + tg::abs(q) + tg::abs(r);
                        if (x == ScalarT(0.0))
                        {
                            continue;
                        }
                        p = p / x;
                        q = q / x;
                        r = r / x;
                    }

                    s = tg::sqrt(p * p + q * q + r * r);
                    if (p < 0)
                    {
                        s = -s;
                    }
                    if (s != 0)
                    {
                        if (k != m)
                        {
                            H[k][k - 1] = -s * x;
                        }
                        else if (l != m)
                        {
                            H[k][k - 1] = -H[k][k - 1];
                        }
                        p = p + s;
                        x = p / s;
                        y = q / s;
                        z = r / s;
                        q = q / p;
                        r = r / p;

                        // Row modification

                        for (int j = k; j < nn; j++)
                        {
                            p = H[k][j] + q * H[k + 1][j];
                            if (notlast)
                            {
                                p = p + r * H[k + 2][j];
                                H[k + 2][j] = H[k + 2][j] - p * z;
                            }
                            H[k][j] = H[k][j] - p * x;
                            H[k + 1][j] = H[k + 1][j] - p * y;
                        }

                        // Column modification

                        for (int i = 0; i <= tg::min(n, k + 3); i++)
                        {
                            p = x * H[i][k] + y * H[i][k + 1];
                            if (notlast)
                            {
                                p = p + z * H[i][k + 2];
                                H[i][k + 2] = H[i][k + 2] - p * r;
                            }
                            H[i][k] = H[i][k] - p;
                            H[i][k + 1] = H[i][k + 1] - p * q;
                        }

                        // Accumulate transformations

                        for (int i = low; i <= high; i++)
                        {
                            p = x * V[i][k] + y * V[i][k + 1];
                            if (notlast)
                            {
                                p = p + z * V[i][k + 2];
                                V[i][k + 2] = V[i][k + 2] - p * r;
                            }
                            V[i][k] = V[i][k] - p;
                            V[i][k + 1] = V[i][k + 1] - p * q;
                        }
                    } // (s != 0)
                }     // k loop
            }         // check convergence
        }             // while (n >= low)

        // Backsubstitute to find vectors of upper triangular form

        if (norm == ScalarT(0.0))
        {
            return;
        }

        for (n = nn - 1; n >= 0; n--)
        {
            p = d[n];
            q = e[n];

            // Real vector

            if (q == 0)
            {
                int l = n;
                H[n][n] = ScalarT(1.0);
                for (int i = n - 1; i >= 0; i--)
                {
                    w = H[i][i] - p;
                    r = ScalarT(0.0);
                    for (int j = l; j <= n; j++)
                    {
                        r = r + H[i][j] * H[j][n];
                    }
                    if (e[i] < ScalarT(0.0))
                    {
                        z = w;
                        s = r;
                    }
                    else
                    {
                        l = i;
                        if (e[i] == ScalarT(0.0))
                        {
                            if (w != ScalarT(0.0))
                            {
                                H[i][n] = -r / w;
                            }
                            else
                            {
                                H[i][n] = -r / (eps * norm);
                            }

                            // Solve real equations
                        }
                        else
                        {
                            x = H[i][i + 1];
                            y = H[i + 1][i];
                            q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                            t = (x * s - z * r) / q;
                            H[i][n] = t;
                            if (tg::abs(x) > tg::abs(z))
                            {
                                H[i + 1][n] = (-r - w * t) / x;
                            }
                            else
                            {
                                H[i + 1][n] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control

                        t = tg::abs(H[i][n]);
                        if ((eps * t) * t > 1)
                        {
                            for (int j = i; j <= n; j++)
                            {
                                H[j][n] = H[j][n] / t;
                            }
                        }
                    }
                }

                // Complex vector
            }
            else if (q < 0)
            {
                int l = n - 1;

                // Last vector component imaginary so matrix is triangular

                if (tg::abs(H[n][n - 1]) > tg::abs(H[n - 1][n]))
                {
                    H[n - 1][n - 1] = q / H[n][n - 1];
                    H[n - 1][n] = -(H[n][n] - p) / H[n][n - 1];
                }
                else
                {
                    cdiv(ScalarT(0.0), -H[n - 1][n], H[n - 1][n - 1] - p, q);
                    H[n - 1][n - 1] = cdivr;
                    H[n - 1][n] = cdivi;
                }
                H[n][n - 1] = ScalarT(0.0);
                H[n][n] = ScalarT(1.0);
                for (int i = n - 2; i >= 0; i--)
                {
                    ScalarT ra, sa, vr, vi;
                    ra = ScalarT(0.0);
                    sa = ScalarT(0.0);
                    for (int j = l; j <= n; j++)
                    {
                        ra = ra + H[i][j] * H[j][n - 1];
                        sa = sa + H[i][j] * H[j][n];
                    }
                    w = H[i][i] - p;

                    if (e[i] < ScalarT(0.0))
                    {
                        z = w;
                        r = ra;
                        s = sa;
                    }
                    else
                    {
                        l = i;
                        if (e[i] == 0)
                        {
                            cdiv(-ra, -sa, w, q);
                            H[i][n - 1] = cdivr;
                            H[i][n] = cdivi;
                        }
                        else
                        {
                            // Solve complex equations

                            x = H[i][i + 1];
                            y = H[i + 1][i];
                            vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                            vi = (d[i] - p) * ScalarT(2.0) * q;
                            if ((vr == ScalarT(0.0)) & (vi == ScalarT(0.0)))
                            {
                                vr = eps * norm * (tg::abs(w) + tg::abs(q) + tg::abs(x) + tg::abs(y) + tg::abs(z));
                            }
                            cdiv(x * r - z * ra + q * sa, x * s - z * sa - q * ra, vr, vi);
                            H[i][n - 1] = cdivr;
                            H[i][n] = cdivi;
                            if (tg::abs(x) > (tg::abs(z) + tg::abs(q)))
                            {
                                H[i + 1][n - 1] = (-ra - w * H[i][n - 1] + q * H[i][n]) / x;
                                H[i + 1][n] = (-sa - w * H[i][n] - q * H[i][n - 1]) / x;
                            }
                            else
                            {
                                cdiv(-r - y * H[i][n - 1], -s - y * H[i][n], z, q);
                                H[i + 1][n - 1] = cdivr;
                                H[i + 1][n] = cdivi;
                            }
                        }

                        // Overflow control

                        t = tg::max(tg::abs(H[i][n - 1]), tg::abs(H[i][n]));
                        if ((eps * t) * t > 1)
                        {
                            for (int j = i; j <= n; j++)
                            {
                                H[j][n - 1] = H[j][n - 1] / t;
                                H[j][n] = H[j][n] / t;
                            }
                        }
                    }
                }
            }
        }

        // Vectors of isolated roots

        for (int i = 0; i < nn; i++)
        {
            if ((i < low) | (i > high))
            {
                for (int j = i; j < nn; j++)
                {
                    V[i][j] = H[i][j];
                }
            }
        }

        // Back transformation to get eigenvectors of original matrix

        for (int j = nn - 1; j >= low; j--)
        {
            for (int i = low; i <= high; i++)
            {
                z = ScalarT(0.0);
                for (int k = low; k <= tg::min(j, high); k++)
                {
                    z = z + V[i][k] * H[k][j];
                }
                V[i][j] = z;
            }
        }
    }


    /* ------------------------
       Constructor
     * ------------------------ */

    /** Check for symmetry, then construct the eigenvalue decomposition
        Structure to access D and V.
    @param Arg    Square matrix
    */
public:
    eigenvalue_decomposition_impl(mat<D, D, ScalarT> const& m)
    {
        // internal matrices are row-major, tg::mat is column major
        mat<D, D, ScalarT> A = transpose(m);

        issymmetric = true;
        for (int j = 0; (j < D) & issymmetric; j++)
        {
            for (int i = 0; (i < D) & issymmetric; i++)
            {
                issymmetric = (A[i][j] == A[j][i]);
            }
        }

        if (issymmetric)
        {
            for (int i = 0; i < D; i++)
            {
                for (int j = 0; j < D; j++)
                {
                    V[i][j] = A[i][j];
                }
            }

            // Tridiagonalize.
            tred2();

            // Diagonalize.
            tql2();
        }
        else
        {
            for (int j = 0; j < D; j++)
            {
                for (int i = 0; i < D; i++)
                {
                    H[i][j] = A[i][j];
                }
            }

            // Reduce to Hessenberg form.
            orthes();

            // Reduce Hessenberg to real Schur form.
            hqr2();
        }
    }

    /* ------------------------
       Public Methods
     * ------------------------ */

    /** Return the eigenvector matrix
    @return     V
    */

    mat<D, D, ScalarT> getV()
    {
        // column vs row-major
        return transpose(mat<D, D, ScalarT>(V));
    }

    /** Return the real parts of the eigenvalues
    @return     real(diag(D))
    */

    array<ScalarT, D> getRealEigenvalues() { return d; }

    /** Return the imaginary parts of the eigenvalues
    @return     imag(diag(D))
    */
    array<ScalarT, D> getImagEigenvalues() { return e; }

    /** Return the block diagonal eigenvalue matrix
    @return     D
    */

    mat<D, D, ScalarT> getD()
    {
        mat<D, D, ScalarT> X;
        for (int i = 0; i < D; i++)
        {
            for (int j = 0; j < D; j++)
            {
                X[i][j] = ScalarT(0.0);
            }
            X[i][i] = d[i];
            if (e[i] > 0)
            {
                X[i][i + 1] = e[i];
            }
            else if (e[i] < 0)
            {
                X[i][i - 1] = e[i];
            }
        }
        // column vs row-major
        return transpose(X);
    }
};

template <int D, class ScalarT>
[[nodiscard]] constexpr array<eigen_decomposition_result<D, ScalarT>, D> eigen_decomposition_impl(mat<D, D, ScalarT> const& m)
{
    eigenvalue_decomposition_impl<D, ScalarT> decomp(m);
    array<eigen_decomposition_result<D, ScalarT>, D> res;

    auto const eigenvalues = decomp.getRealEigenvalues();
    auto const eigenvectors = decomp.getV();

    for (auto i = 0; i < D; ++i)
        res[i] = {eigenvectors[i], eigenvalues[i]};

    return res;
}
}

template <>
[[nodiscard]] inline constexpr array<eigen_decomposition_result<3, float>, 3> eigen_decomposition(mat<3, 3, float> const& m)
{
    return detail::eigen_decomposition_impl(m);
}

template <>
[[nodiscard]] inline constexpr array<eigen_decomposition_result<4, float>, 4> eigen_decomposition(mat<4, 4, float> const& m)
{
    return detail::eigen_decomposition_impl(m);
}

template <>
[[nodiscard]] inline constexpr array<eigen_decomposition_result<3, double>, 3> eigen_decomposition(mat<3, 3, double> const& m)
{
    return detail::eigen_decomposition_impl(m);
}

template <>
[[nodiscard]] inline constexpr array<eigen_decomposition_result<4, double>, 4> eigen_decomposition(mat<4, 4, double> const& m)
{
    return detail::eigen_decomposition_impl(m);
}


}
