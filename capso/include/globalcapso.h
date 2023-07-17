#pragma once

#include <random>
#include <list>

#include "cellularautomaton.h"
#include "swarm.h"
#include "capso_export.h"


namespace capso
{
    class CAPSO_EXPORT Global final : public capso::CellularAutomaton
    {
        public:
            enum State {EMPTY, PREY, PREDATOR, PREY_PREDATOR, BEST = 4};

            Global(int width, int height);

            void setInitialPreyPercentage(float value);
            void setCompetitionFactor(float value);
            void setCompetitionRadius(int value);
            void setPreyMeanOffspring(int value);
            void setPreyReproductionRadius(int value);
            void setPredatorMeanOffspring(int value);
            void setPredatorReproductionRadius(int value);
            void setInitialSwarmSize(int value);
            void setCognitiveFactor(float value);
            void setSocialFactor(float value);
            void setMaximumSpeed(int value);
            void setMitrationTime(int value);
            void setInitialInertialWeight(float value);
            void setFinalInertiaWeight(float value);

            int numberOfPreys() const;
            int numberOfPredators() const;

            void initialize() override;
            virtual void clear() override;
            void nextGen() override;

        private:
            Global(const Global&);
            Global& operator=(const Global&);

        private:
            Swarm mPredatorSwarm;

            // Containers
            std::vector<unsigned char> mTemp;
            std::vector<unsigned char> mPreyDensities;

            int mNumberOfPreys, mNumberOfPredators;

            RandomNumber mRandom;

            // A function pointer that handles transitions
            void (Global::*mNextStage)();

            // Model parameters
            double mInitialPreyPercentage;
            double mCompetitionFactor;
            int mCompetitionRadius;
            int mPreyMeanOffspring;
            int mPreyReproductionRadius;
            int mPredatorMeanOffspring;
            int mPredatorReproductionRadius;
            int NEIGHBORHOOD_SIZE;

            // PSO parameters
            int mInitialSwarmSize;
            int mMigrationTime;
            int mMigrationCount;
            float mInitialInertiaWeight;
            float mCurrentInertiaWeight;
            float mFinalInertiaWeight;
            const float INERTIA_STEP;
            LatticePoint mBestPosition;

            // Model Stages
            void competitionOfPreys();
            void migration();
            void reproductionOfPredators();
            void predatorsDeath();
            void predation();
            void predation2();
            void reproductionOfPreys();

            // Misc methods
            void notifyNeighbors(const int& row, const int& col,
                                 const bool& death_birth);

            bool checkState(int address, State state);
            void setState(int address, State state);
            void clearState(int address, State state);
    };
}
