#include "eigenvalues.hh"

#include <cmath> // std::hypot

#include <typed-geometry/detail/special_values.hh>

// This file includes an adapted version of the Eigendecomposition class from Jama (https://math.nist.gov/javanumerics/jama/)
// which is in public domain.

namespace
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
    bool m_is_symmetric;

    /** Arrays for internal storage of eigenvalues.
    @serial internal storage of eigenvalues.
    */
    tg::array<ScalarT, D> m_d;
    tg::array<ScalarT, D> m_e;

    /** Array for internal storage of eigenvectors.
    @serial internal storage of eigenvectors.
    */
    tg::array<tg::array<ScalarT, D>, D> m_V;

    /** Array for internal storage of nonsymmetric Hessenberg form.
    @serial internal storage of nonsymmetric Hessenberg form.
    */
    tg::array<tg::array<ScalarT, D>, D> m_H;

    /** Working storage for nonsymmetric algorithm.
    @serial working storage for nonsymmetric algorithm.
    */
    tg::array<ScalarT, D> m_ort;

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
            m_d[j] = m_V[D - 1][j];
        }

        // Householder reduction to tridiagonal form.

        for (int i = D - 1; i > 0; i--)
        {
            // Scale to avoid under/overflow.

            ScalarT scale = ScalarT(0.0);
            ScalarT h = ScalarT(0.0);
            for (int k = 0; k < i; k++)
            {
                scale = scale + tg::abs(m_d[k]);
            }
            if (scale == ScalarT(0.0))
            {
                m_e[i] = m_d[i - 1];
                for (int j = 0; j < i; j++)
                {
                    m_d[j] = m_V[i - 1][j];
                    m_V[i][j] = ScalarT(0.0);
                    m_V[j][i] = ScalarT(0.0);
                }
            }
            else
            {
                // Generate Householder vector.

                for (int k = 0; k < i; k++)
                {
                    m_d[k] /= scale;
                    h += m_d[k] * m_d[k];
                }
                ScalarT f = m_d[i - 1];
                ScalarT g = tg::sqrt(h);
                if (f > 0)
                {
                    g = -g;
                }
                m_e[i] = scale * g;
                h = h - f * g;
                m_d[i - 1] = f - g;
                for (int j = 0; j < i; j++)
                {
                    m_e[j] = ScalarT(0.0);
                }

                // Apply similarity transformation to remaining columns.

                for (int j = 0; j < i; j++)
                {
                    f = m_d[j];
                    m_V[j][i] = f;
                    g = m_e[j] + m_V[j][j] * f;
                    for (int k = j + 1; k <= i - 1; k++)
                    {
                        g += m_V[k][j] * m_d[k];
                        m_e[k] += m_V[k][j] * f;
                    }
                    m_e[j] = g;
                }
                f = ScalarT(0.0);
                for (int j = 0; j < i; j++)
                {
                    m_e[j] /= h;
                    f += m_e[j] * m_d[j];
                }
                ScalarT hh = f / (h + h);
                for (int j = 0; j < i; j++)
                {
                    m_e[j] -= hh * m_d[j];
                }
                for (int j = 0; j < i; j++)
                {
                    f = m_d[j];
                    g = m_e[j];
                    for (int k = j; k <= i - 1; k++)
                    {
                        m_V[k][j] -= (f * m_e[k] + g * m_d[k]);
                    }
                    m_d[j] = m_V[i - 1][j];
                    m_V[i][j] = ScalarT(0.0);
                }
            }
            m_d[i] = h;
        }

        // Accumulate transformations.

        for (int i = 0; i < D - 1; i++)
        {
            m_V[D - 1][i] = m_V[i][i];
            m_V[i][i] = ScalarT(1.0);
            ScalarT h = m_d[i + 1];
            if (h != ScalarT(0.0))
            {
                for (int k = 0; k <= i; k++)
                {
                    m_d[k] = m_V[k][i + 1] / h;
                }
                for (int j = 0; j <= i; j++)
                {
                    ScalarT g = ScalarT(0.0);
                    for (int k = 0; k <= i; k++)
                    {
                        g += m_V[k][i + 1] * m_V[k][j];
                    }
                    for (int k = 0; k <= i; k++)
                    {
                        m_V[k][j] -= g * m_d[k];
                    }
                }
            }
            for (int k = 0; k <= i; k++)
            {
                m_V[k][i + 1] = ScalarT(0.0);
            }
        }
        for (int j = 0; j < D; j++)
        {
            m_d[j] = m_V[D - 1][j];
            m_V[D - 1][j] = ScalarT(0.0);
        }
        m_V[D - 1][D - 1] = ScalarT(1.0);
        m_e[0] = ScalarT(0.0);
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
            m_e[i - 1] = m_e[i];
        }
        m_e[D - 1] = ScalarT(0.0);

        ScalarT f = ScalarT(0.0);
        ScalarT tst1 = ScalarT(0.0);
        ScalarT eps = tg::epsilon<ScalarT>;

        for (int l = 0; l < D; l++)
        {
            // Find small subdiagonal element

            tst1 = tg::max(tst1, tg::abs(m_d[l]) + tg::abs(m_e[l]));
            int m = l;
            while (m < D)
            {
                if (tg::abs(m_e[m]) <= eps * tst1)
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

                    ScalarT g = m_d[l];
                    ScalarT p = (m_d[l + 1] - g) / (ScalarT(2.0) * m_e[l]);
                    ScalarT r = std::hypot(p, ScalarT(1.0));
                    if (p < 0)
                    {
                        r = -r;
                    }
                    m_d[l] = m_e[l] / (p + r);
                    m_d[l + 1] = m_e[l] * (p + r);
                    ScalarT dl1 = m_d[l + 1];
                    ScalarT h = g - m_d[l];
                    for (int i = l + 2; i < D; i++)
                    {
                        m_d[i] -= h;
                    }
                    f = f + h;

                    // Implicit QL transformation.

                    p = m_d[m];
                    ScalarT c = ScalarT(1.0);
                    ScalarT c2 = c;
                    ScalarT c3 = c;
                    ScalarT el1 = m_e[l + 1];
                    ScalarT s = ScalarT(0.0);
                    ScalarT s2 = ScalarT(0.0);
                    for (int i = m - 1; i >= l; i--)
                    {
                        c3 = c2;
                        c2 = c;
                        s2 = s;
                        g = c * m_e[i];
                        h = c * p;
                        r = std::hypot(p, m_e[i]);
                        m_e[i + 1] = s * r;
                        s = m_e[i] / r;
                        c = p / r;
                        p = c * m_d[i] - s * g;
                        m_d[i + 1] = h + s * (c * g + s * m_d[i]);

                        // Accumulate transformation.

                        for (int k = 0; k < D; k++)
                        {
                            h = m_V[k][i + 1];
                            m_V[k][i + 1] = s * m_V[k][i] + c * h;
                            m_V[k][i] = c * m_V[k][i] - s * h;
                        }
                    }
                    p = -s * s2 * c3 * el1 * m_e[l] / dl1;
                    m_e[l] = s * p;
                    m_d[l] = c * p;

                    // Check for convergence.

                } while (tg::abs(m_e[l]) > eps * tst1);
            }
            m_d[l] = m_d[l] + f;
            m_e[l] = ScalarT(0.0);
        }

        // Sort eigenvalues and corresponding vectors.

        for (int i = 0; i < D - 1; i++)
        {
            int k = i;
            ScalarT p = m_d[i];
            for (int j = i + 1; j < D; j++)
            {
                if (m_d[j] < p)
                {
                    k = j;
                    p = m_d[j];
                }
            }
            if (k != i)
            {
                m_d[k] = m_d[i];
                m_d[i] = p;
                for (int j = 0; j < D; j++)
                {
                    p = m_V[j][i];
                    m_V[j][i] = m_V[j][k];
                    m_V[j][k] = p;
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
                scale = scale + tg::abs(m_H[i][m - 1]);
            }
            if (scale != ScalarT(0.0))
            {
                // Compute Householder transformation.

                ScalarT h = ScalarT(0.0);
                for (int i = high; i >= m; i--)
                {
                    m_ort[i] = m_H[i][m - 1] / scale;
                    h += m_ort[i] * m_ort[i];
                }
                ScalarT g = tg::sqrt(h);
                if (m_ort[m] > 0)
                {
                    g = -g;
                }
                h = h - m_ort[m] * g;
                m_ort[m] = m_ort[m] - g;

                // Apply Householder similarity transformation
                // H = (I-u*u'/h)*H*(I-u*u')/h)

                for (int j = m; j < D; j++)
                {
                    ScalarT f = ScalarT(0.0);
                    for (int i = high; i >= m; i--)
                    {
                        f += m_ort[i] * m_H[i][j];
                    }
                    f = f / h;
                    for (int i = m; i <= high; i++)
                    {
                        m_H[i][j] -= f * m_ort[i];
                    }
                }

                for (int i = 0; i <= high; i++)
                {
                    ScalarT f = ScalarT(0.0);
                    for (int j = high; j >= m; j--)
                    {
                        f += m_ort[j] * m_H[i][j];
                    }
                    f = f / h;
                    for (int j = m; j <= high; j++)
                    {
                        m_H[i][j] -= f * m_ort[j];
                    }
                }
                m_ort[m] = scale * m_ort[m];
                m_H[m][m - 1] = scale * g;
            }
        }

        // Accumulate transformations (Algol's ortran).

        for (int i = 0; i < D; i++)
        {
            for (int j = 0; j < D; j++)
            {
                m_V[i][j] = (i == j ? ScalarT(1.0) : ScalarT(0.0));
            }
        }

        for (int m = high - 1; m >= low + 1; m--)
        {
            if (m_H[m][m - 1] != ScalarT(0.0))
            {
                for (int i = m + 1; i <= high; i++)
                {
                    m_ort[i] = m_H[i][m - 1];
                }
                for (int j = m; j <= high; j++)
                {
                    ScalarT g = ScalarT(0.0);
                    for (int i = m; i <= high; i++)
                    {
                        g += m_ort[i] * m_V[i][j];
                    }
                    // ScalarT division avoids possible underflow
                    g = (g / m_ort[m]) / m_H[m][m - 1];
                    for (int i = m; i <= high; i++)
                    {
                        m_V[i][j] += g * m_ort[i];
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
        ScalarT eps = tg::epsilon<ScalarT>;
        ScalarT exshift = ScalarT(0.0);
        ScalarT p = 0, q = 0, r = 0, s = 0, z = 0, t, w, x, y;

        // Store roots isolated by balanc and compute matrix norm

        ScalarT norm = ScalarT(0.0);
        for (int i = 0; i < nn; i++)
        {
            if ((i < low) | (i > high))
            {
                m_d[i] = m_H[i][i];
                m_e[i] = ScalarT(0.0);
            }
            for (int j = tg::max(i - 1, 0); j < nn; j++)
            {
                norm = norm + tg::abs(m_H[i][j]);
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
                s = tg::abs(m_H[l - 1][l - 1]) + tg::abs(m_H[l][l]);
                if (s == ScalarT(0.0))
                {
                    s = norm;
                }
                if (tg::abs(m_H[l][l - 1]) < eps * s)
                {
                    break;
                }
                l--;
            }

            // Check for convergence
            // One root found

            if (l == n)
            {
                m_H[n][n] = m_H[n][n] + exshift;
                m_d[n] = m_H[n][n];
                m_e[n] = ScalarT(0.0);
                n--;
                iter = 0;

                // Two roots found
            }
            else if (l == n - 1)
            {
                w = m_H[n][n - 1] * m_H[n - 1][n];
                p = (m_H[n - 1][n - 1] - m_H[n][n]) / ScalarT(2.0);
                q = p * p + w;
                z = tg::sqrt(tg::abs(q));
                m_H[n][n] = m_H[n][n] + exshift;
                m_H[n - 1][n - 1] = m_H[n - 1][n - 1] + exshift;
                x = m_H[n][n];

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
                    m_d[n - 1] = x + z;
                    m_d[n] = m_d[n - 1];
                    if (z != ScalarT(0.0))
                    {
                        m_d[n] = x - w / z;
                    }
                    m_e[n - 1] = ScalarT(0.0);
                    m_e[n] = ScalarT(0.0);
                    x = m_H[n][n - 1];
                    s = tg::abs(x) + tg::abs(z);
                    p = x / s;
                    q = z / s;
                    r = tg::sqrt(p * p + q * q);
                    p = p / r;
                    q = q / r;

                    // Row modification

                    for (int j = n - 1; j < nn; j++)
                    {
                        z = m_H[n - 1][j];
                        m_H[n - 1][j] = q * z + p * m_H[n][j];
                        m_H[n][j] = q * m_H[n][j] - p * z;
                    }

                    // Column modification

                    for (int i = 0; i <= n; i++)
                    {
                        z = m_H[i][n - 1];
                        m_H[i][n - 1] = q * z + p * m_H[i][n];
                        m_H[i][n] = q * m_H[i][n] - p * z;
                    }

                    // Accumulate transformations

                    for (int i = low; i <= high; i++)
                    {
                        z = m_V[i][n - 1];
                        m_V[i][n - 1] = q * z + p * m_V[i][n];
                        m_V[i][n] = q * m_V[i][n] - p * z;
                    }

                    // Complex pair
                }
                else
                {
                    m_d[n - 1] = x + p;
                    m_d[n] = x + p;
                    m_e[n - 1] = z;
                    m_e[n] = -z;
                }
                n = n - 2;
                iter = 0;

                // No convergence yet
            }
            else
            {
                // Form shift

                x = m_H[n][n];
                y = ScalarT(0.0);
                w = ScalarT(0.0);
                if (l < n)
                {
                    y = m_H[n - 1][n - 1];
                    w = m_H[n][n - 1] * m_H[n - 1][n];
                }

                // Wilkinson's original ad hoc shift

                if (iter == 10)
                {
                    exshift += x;
                    for (int i = low; i <= n; i++)
                    {
                        m_H[i][i] -= x;
                    }
                    s = tg::abs(m_H[n][n - 1]) + tg::abs(m_H[n - 1][n - 2]);
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
                            m_H[i][i] -= s;
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
                    z = m_H[m][m];
                    r = x - z;
                    s = y - z;
                    p = (r * s - w) / m_H[m + 1][m] + m_H[m][m + 1];
                    q = m_H[m + 1][m + 1] - z - r - s;
                    r = m_H[m + 2][m + 1];
                    s = tg::abs(p) + tg::abs(q) + tg::abs(r);
                    p = p / s;
                    q = q / s;
                    r = r / s;
                    if (m == l)
                    {
                        break;
                    }
                    if (tg::abs(m_H[m][m - 1]) * (tg::abs(q) + tg::abs(r))
                        < eps * (tg::abs(p) * (tg::abs(m_H[m - 1][m - 1]) + tg::abs(z) + tg::abs(m_H[m + 1][m + 1]))))
                    {
                        break;
                    }
                    m--;
                }

                for (int i = m + 2; i <= n; i++)
                {
                    m_H[i][i - 2] = ScalarT(0.0);
                    if (i > m + 2)
                    {
                        m_H[i][i - 3] = ScalarT(0.0);
                    }
                }

                // ScalarT QR step involving rows l:n and columns m:n


                for (int k = m; k <= n - 1; k++)
                {
                    bool notlast = (k != n - 1);
                    if (k != m)
                    {
                        p = m_H[k][k - 1];
                        q = m_H[k + 1][k - 1];
                        r = (notlast ? m_H[k + 2][k - 1] : ScalarT(0.0));
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
                            m_H[k][k - 1] = -s * x;
                        }
                        else if (l != m)
                        {
                            m_H[k][k - 1] = -m_H[k][k - 1];
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
                            p = m_H[k][j] + q * m_H[k + 1][j];
                            if (notlast)
                            {
                                p = p + r * m_H[k + 2][j];
                                m_H[k + 2][j] = m_H[k + 2][j] - p * z;
                            }
                            m_H[k][j] = m_H[k][j] - p * x;
                            m_H[k + 1][j] = m_H[k + 1][j] - p * y;
                        }

                        // Column modification

                        for (int i = 0; i <= tg::min(n, k + 3); i++)
                        {
                            p = x * m_H[i][k] + y * m_H[i][k + 1];
                            if (notlast)
                            {
                                p = p + z * m_H[i][k + 2];
                                m_H[i][k + 2] = m_H[i][k + 2] - p * r;
                            }
                            m_H[i][k] = m_H[i][k] - p;
                            m_H[i][k + 1] = m_H[i][k + 1] - p * q;
                        }

                        // Accumulate transformations

                        for (int i = low; i <= high; i++)
                        {
                            p = x * m_V[i][k] + y * m_V[i][k + 1];
                            if (notlast)
                            {
                                p = p + z * m_V[i][k + 2];
                                m_V[i][k + 2] = m_V[i][k + 2] - p * r;
                            }
                            m_V[i][k] = m_V[i][k] - p;
                            m_V[i][k + 1] = m_V[i][k + 1] - p * q;
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
            p = m_d[n];
            q = m_e[n];

            // Real vector

            if (q == 0)
            {
                int l = n;
                m_H[n][n] = ScalarT(1.0);
                for (int i = n - 1; i >= 0; i--)
                {
                    w = m_H[i][i] - p;
                    r = ScalarT(0.0);
                    for (int j = l; j <= n; j++)
                    {
                        r = r + m_H[i][j] * m_H[j][n];
                    }
                    if (m_e[i] < ScalarT(0.0))
                    {
                        z = w;
                        s = r;
                    }
                    else
                    {
                        l = i;
                        if (m_e[i] == ScalarT(0.0))
                        {
                            if (w != ScalarT(0.0))
                            {
                                m_H[i][n] = -r / w;
                            }
                            else
                            {
                                m_H[i][n] = -r / (eps * norm);
                            }

                            // Solve real equations
                        }
                        else
                        {
                            x = m_H[i][i + 1];
                            y = m_H[i + 1][i];
                            q = (m_d[i] - p) * (m_d[i] - p) + m_e[i] * m_e[i];
                            t = (x * s - z * r) / q;
                            m_H[i][n] = t;
                            if (tg::abs(x) > tg::abs(z))
                            {
                                m_H[i + 1][n] = (-r - w * t) / x;
                            }
                            else
                            {
                                m_H[i + 1][n] = (-s - y * t) / z;
                            }
                        }

                        // Overflow control

                        t = tg::abs(m_H[i][n]);
                        if ((eps * t) * t > 1)
                        {
                            for (int j = i; j <= n; j++)
                            {
                                m_H[j][n] = m_H[j][n] / t;
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

                if (tg::abs(m_H[n][n - 1]) > tg::abs(m_H[n - 1][n]))
                {
                    m_H[n - 1][n - 1] = q / m_H[n][n - 1];
                    m_H[n - 1][n] = -(m_H[n][n] - p) / m_H[n][n - 1];
                }
                else
                {
                    cdiv(ScalarT(0.0), -m_H[n - 1][n], m_H[n - 1][n - 1] - p, q);
                    m_H[n - 1][n - 1] = cdivr;
                    m_H[n - 1][n] = cdivi;
                }
                m_H[n][n - 1] = ScalarT(0.0);
                m_H[n][n] = ScalarT(1.0);
                for (int i = n - 2; i >= 0; i--)
                {
                    ScalarT ra, sa, vr, vi;
                    ra = ScalarT(0.0);
                    sa = ScalarT(0.0);
                    for (int j = l; j <= n; j++)
                    {
                        ra = ra + m_H[i][j] * m_H[j][n - 1];
                        sa = sa + m_H[i][j] * m_H[j][n];
                    }
                    w = m_H[i][i] - p;

                    if (m_e[i] < ScalarT(0.0))
                    {
                        z = w;
                        r = ra;
                        s = sa;
                    }
                    else
                    {
                        l = i;
                        if (m_e[i] == 0)
                        {
                            cdiv(-ra, -sa, w, q);
                            m_H[i][n - 1] = cdivr;
                            m_H[i][n] = cdivi;
                        }
                        else
                        {
                            // Solve complex equations

                            x = m_H[i][i + 1];
                            y = m_H[i + 1][i];
                            vr = (m_d[i] - p) * (m_d[i] - p) + m_e[i] * m_e[i] - q * q;
                            vi = (m_d[i] - p) * ScalarT(2.0) * q;
                            if ((vr == ScalarT(0.0)) & (vi == ScalarT(0.0)))
                            {
                                vr = eps * norm * (tg::abs(w) + tg::abs(q) + tg::abs(x) + tg::abs(y) + tg::abs(z));
                            }
                            cdiv(x * r - z * ra + q * sa, x * s - z * sa - q * ra, vr, vi);
                            m_H[i][n - 1] = cdivr;
                            m_H[i][n] = cdivi;
                            if (tg::abs(x) > (tg::abs(z) + tg::abs(q)))
                            {
                                m_H[i + 1][n - 1] = (-ra - w * m_H[i][n - 1] + q * m_H[i][n]) / x;
                                m_H[i + 1][n] = (-sa - w * m_H[i][n] - q * m_H[i][n - 1]) / x;
                            }
                            else
                            {
                                cdiv(-r - y * m_H[i][n - 1], -s - y * m_H[i][n], z, q);
                                m_H[i + 1][n - 1] = cdivr;
                                m_H[i + 1][n] = cdivi;
                            }
                        }

                        // Overflow control

                        t = tg::max(tg::abs(m_H[i][n - 1]), tg::abs(m_H[i][n]));
                        if ((eps * t) * t > 1)
                        {
                            for (int j = i; j <= n; j++)
                            {
                                m_H[j][n - 1] = m_H[j][n - 1] / t;
                                m_H[j][n] = m_H[j][n] / t;
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
                    m_V[i][j] = m_H[i][j];
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
                    z = z + m_V[i][k] * m_H[k][j];
                }
                m_V[i][j] = z;
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
    eigenvalue_decomposition_impl(tg::mat<D, D, ScalarT> const& m)
    {
        // internal matrices are row-major, tg::mat is column major
        tg::mat<D, D, ScalarT> A = transpose(m);

        m_is_symmetric = true;
        for (int j = 0; (j < D) & m_is_symmetric; j++)
        {
            for (int i = 0; (i < D) & m_is_symmetric; i++)
            {
                m_is_symmetric = (A[i][j] == A[j][i]);
            }
        }

        if (m_is_symmetric)
        {
            for (int i = 0; i < D; i++)
            {
                for (int j = 0; j < D; j++)
                {
                    m_V[i][j] = A[i][j];
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
                    m_H[i][j] = A[i][j];
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

    tg::mat<D, D, ScalarT> getV()
    {
        // column vs row-major
        return transpose(tg::mat<D, D, ScalarT>(m_V));
    }

    /** Return the real parts of the eigenvalues
    @return     real(diag(D))
    */

    tg::array<ScalarT, D> getRealEigenvalues() { return m_d; }

    /** Return the imaginary parts of the eigenvalues
    @return     imag(diag(D))
    */
    tg::array<ScalarT, D> getImagEigenvalues() { return m_e; }

    /** Return the block diagonal eigenvalue matrix
    @return     D
    */

    tg::mat<D, D, ScalarT> getD()
    {
        tg::mat<D, D, ScalarT> X;
        for (int i = 0; i < D; i++)
        {
            for (int j = 0; j < D; j++)
            {
                X[i][j] = ScalarT(0.0);
            }
            X[i][i] = m_d[i];
            if (m_e[i] > 0)
            {
                X[i][i + 1] = m_e[i];
            }
            else if (m_e[i] < 0)
            {
                X[i][i - 1] = m_e[i];
            }
        }
        // column vs row-major
        return transpose(X);
    }
};

template <int D, class ScalarT>
tg::array<tg::eigen_decomposition_result<D, ScalarT>, D> eigen_decomp_wrapper(tg::mat<D, D, ScalarT> const& m)
{
    eigenvalue_decomposition_impl<D, ScalarT> decomp(m);
    tg::array<tg::eigen_decomposition_result<D, ScalarT>, D> res;

    auto const eigenvalues = decomp.getRealEigenvalues();
    auto const eigenvectors = decomp.getV();

    for (auto i = 0; i < D; ++i)
        res[i] = {eigenvectors[i], eigenvalues[i]};

    return res;
}

template <int D, class ScalarT>
tg::array<ScalarT, D> eigenvalues_wrapper(tg::mat<D, D, ScalarT> const& m)
{
    eigenvalue_decomposition_impl<D, ScalarT> decomp(m);
    tg::array<ScalarT, D> res;

    auto const eigenvalues = decomp.getRealEigenvalues();

    for (auto i = 0; i < D; ++i)
        res[i] = {eigenvalues[i]};

    return res;
}

template <int D, class ScalarT>
tg::array<tg::vec<D, ScalarT>, D> eigenvectors_wrapper(tg::mat<D, D, ScalarT> const& m)
{
    eigenvalue_decomposition_impl<D, ScalarT> decomp(m);
    tg::array<tg::vec<D, ScalarT>, D> res;

    auto const eigenvectors = decomp.getV();

    for (auto i = 0; i < D; ++i)
        res[i] = {eigenvectors[i]};

    return res;
}
}
namespace tg::detail
{
array<eigen_decomposition_result<2, float>, 2> eigen_decomposition_impl(mat<2, 2, float> const& m) { return eigen_decomp_wrapper(m); }
array<eigen_decomposition_result<2, double>, 2> eigen_decomposition_impl(mat<2, 2, double> const& m) { return eigen_decomp_wrapper(m); }
array<eigen_decomposition_result<3, float>, 3> eigen_decomposition_impl(mat<3, 3, float> const& m) { return eigen_decomp_wrapper(m); }
array<eigen_decomposition_result<3, double>, 3> eigen_decomposition_impl(mat<3, 3, double> const& m) { return eigen_decomp_wrapper(m); }
array<eigen_decomposition_result<4, float>, 4> eigen_decomposition_impl(mat<4, 4, float> const& m) { return eigen_decomp_wrapper(m); }
array<eigen_decomposition_result<4, double>, 4> eigen_decomposition_impl(mat<4, 4, double> const& m) { return eigen_decomp_wrapper(m); }
array<float, 2> eigenvalues_impl(mat<2, 2, float> const& m) { return eigenvalues_wrapper(m); }
array<double, 2> eigenvalues_impl(mat<2, 2, double> const& m) { return eigenvalues_wrapper(m); }
array<float, 3> eigenvalues_impl(mat<3, 3, float> const& m) { return eigenvalues_wrapper(m); }
array<double, 3> eigenvalues_impl(mat<3, 3, double> const& m) { return eigenvalues_wrapper(m); }
array<float, 4> eigenvalues_impl(mat<4, 4, float> const& m) { return eigenvalues_wrapper(m); }
array<double, 4> eigenvalues_impl(mat<4, 4, double> const& m) { return eigenvalues_wrapper(m); }
array<vec<2, float>, 2> eigenvectors_impl(mat<2, 2, float> const& m) { return eigenvectors_wrapper(m); }
array<vec<2, double>, 2> eigenvectors_impl(mat<2, 2, double> const& m) { return eigenvectors_wrapper(m); }
array<vec<3, float>, 3> eigenvectors_impl(mat<3, 3, float> const& m) { return eigenvectors_wrapper(m); }
array<vec<3, double>, 3> eigenvectors_impl(mat<3, 3, double> const& m) { return eigenvectors_wrapper(m); }
array<vec<4, float>, 4> eigenvectors_impl(mat<4, 4, float> const& m) { return eigenvectors_wrapper(m); }
array<vec<4, double>, 4> eigenvectors_impl(mat<4, 4, double> const& m) { return eigenvectors_wrapper(m); }
}
