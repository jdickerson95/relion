#ifndef TILT_REFINEMENT_H
#define TILT_REFINEMENT_H

#include <src/ctf.h>
#include <src/image.h>
#include <src/metadata_table.h>
#include <src/jaz/optimization/optimization.h>
#include <src/jaz/volume.h>
#include <src/jaz/gravis/t2Matrix.h>
#include <vector>

class TiltOptimization : public Optimization
{
    public:

        TiltOptimization(
                const Image<Complex>& xy,
                const Image<RFLOAT>& weight,
                double angpix,
                bool L1 = false,
                bool anisotropic = false);

        double f(const std::vector<double>& x, void* tempStorage) const;

    private:

        const Image<Complex>& xy;
        const Image<RFLOAT>& weight;
        const double angpix;
        const bool L1, anisotropic;
};

class BasisOptimisation : public Optimization
{
    public:

        BasisOptimisation(
                const Image<Complex>& xy,
                const Image<RFLOAT>& weight,
                const std::vector<Image<RFLOAT>>& basis,
                bool L1 = false);

        double f(const std::vector<double>& x, void* tempStorage) const;
		
		void* allocateTempStorage() const;		
		void deallocateTempStorage(void* ts);

    private:

		const int w, h, cc;
        const Image<Complex>& xy;
        const Image<RFLOAT>& weight;
		const std::vector<Image<RFLOAT>>& basis;
        const bool L1;
};

class TiltHelper
{
    public:

        static void updateTiltShift(
                const Image<Complex>& prediction,
                const Image<Complex>& observation,
                const CTF& ctf, double angpix,
                Image<Complex>& xyDest,
                Image<RFLOAT>& wDest);

        static void updateTiltShiftPar(
                const Image<Complex>& prediction,
                const Image<Complex>& observation,
                const CTF& ctf, double angpix,
                Image<Complex>& xyDest,
                Image<RFLOAT>& wDest);

        static void fitTiltShift(
                const Image<RFLOAT>& phase,
                const Image<RFLOAT>& weight,
                double Cs, double lambda, double angpix,
                double* shift_x, double* shift_y,
                double* tilt_x, double* tilt_y,
                Image<RFLOAT>* fit,
                gravis::d2Matrix magCorr = gravis::d2Matrix());

        static void optimizeTilt(
                const Image<Complex>& xy,
                const Image<RFLOAT>& weight,
                double Cs, double lambda, double angpix,
                bool L1,
                double shift0_x, double shift0_y,
                double tilt0_x, double tilt0_y,
                double* shift_x, double* shift_y,
                double* tilt_x, double* tilt_y,
                Image<RFLOAT>* fit);
		
		
		static std::vector<double> fitOddZernike(
				const Image<Complex>& xy,
                const Image<RFLOAT>& weight,
				double angpix, int n_max, 
				Image<RFLOAT>* fit = 0);
		
		static std::vector<double> optimiseOddZernike(
                const Image<Complex>& xy,
                const Image<RFLOAT>& weight,
                double angpix, int n_max,
                const std::vector<double>& coeffs,
                Image<RFLOAT>* fit);
		
		static std::vector<Image<RFLOAT>> computeOddZernike(
				int s, double angpix, int n_max);
		
		
		static Image<RFLOAT> plotOddZernike(
				const std::vector<double>& coeffs,
				int s, double angpix);
		
		static Image<RFLOAT> plotTilt(
				double tx, double ty, int s, double angpix, 
				double Cs, double lambda);
		
		
		static void extractTilt(
				std::vector<double>& oddZernikeCoeffs,
				double& tilt_x, double& tilt_y, 
				double Cs, double lambda);
		
		static void insertTilt(
				std::vector<double>& oddZernikeCoeffs,
				double tilt_x, double tilt_y, 
				double Cs, double lambda);
		
		
		static std::vector<double> fitBasisLin(
				const Image<Complex>& xy,
                const Image<RFLOAT>& weight,
				const std::vector<Image<RFLOAT>>& basis);
		
		static std::vector<double> optimiseBasis(
				const Image<Complex>& xy,
                const Image<RFLOAT>& weight,
				const std::vector<Image<RFLOAT>>& basis,
				const std::vector<double>& initial);

        static void optimizeAnisoTilt(
                const Image<Complex>& xy,
                const Image<RFLOAT>& weight,
                double Cs, double lambda, double angpix,
                bool L1,
                double shift0_x, double shift0_y,
                double tilt0_x, double tilt0_y,
                double* shift_x, double* shift_y,
                double* tilt_x, double* tilt_y,
                double* tilt_xx, double* tilt_xy, double* tilt_yy,
                Image<RFLOAT>* fit);

        static void drawPhaseShift(
                double shift_x, double shift_y,
                double tilt_x, double tilt_y,
                int w, int h, double as,
                gravis::d2Matrix magCorr,
                Image<RFLOAT>* tgt);

        static void drawPhaseShift(
                double shift_x, double shift_y,
                double tilt_x, double tilt_y,
                double tilt_xx, double tilt_xy, double tilt_yy,
                int w, int h, double as,
                gravis::d2Matrix magCorr,
                Image<RFLOAT>* tgt);

};

#endif