#pragma once

#include <random>
#include <list>

#include "cellularautomaton.h"
#include "swarm.h"
#include "capsosettings.h"
#include "capso_export.h"


namespace capso
{
    class CAPSO_EXPORT Local final : public capso::CellularAutomaton
    {
        public:
            enum State { EMPTY, PREY, PREDATOR, PREY_PREDATOR };
            enum Stage { COMPETITION, MIGRATION, REPRODUCTION_OF_PREDATORS,
                DEATH_OF_PREDATORS, DEATH_OF_PREYS, REPRODUCTION_OF_PREYS };

            Local(int width, int height);

            void initialize() override;
            virtual void clear() override;
            void nextGen() override;

            void setPredatorMigrationTime(int value);

            void setSettings(const CaPsoSettings& settings);
            CaPsoSettings settings() const;

            int   numberOfPreys() const;
            int   numberOfPredators() const;
            float preyBirthRate() const;
            float predatorBirthRate() const;
            float preyDeathProbability() const;
            float predatorDeathProbability() const;
            int   currentStage() const;

        private:
            Local(const Local&);
            Local& operator=(const Local&);

        private:
            // Model stages
            void competitionOfPreys();
            void migration();
            void reproductionOfPredators();
            void predatorsDeath();
            void predation();
            void reproductionOfPreys();

            // Misc methods
            void notifyNeighbors(const int& row, const int& col,
                                 const bool& death_birth);
            bool checkState(int address, State state);
            void setState(int address, State state);
            void clearState(int address, State state);

            // Containers
            std::vector<unsigned char> mPreyDensities;
            std::vector<unsigned char> mTemp;

            capso::Swarm mPredatorSwarm;

            // Metrics
            int mNumberOfPreys              { 0 };
            int mNumberOfPredators          { 0 };
            float mPreyBirthRate            { 0.0f };
            float mPredatorBirthRate        { 0.0f };
            float mPreyDeathProbability     { 0.0f };
            float mPredatorDeathProbability { 0.0f };
            int mCurrentStage               { COMPETITION };

            capso::RandomNumber mRandom;

            // A function pointer that handles transitions
            void (Local::*mNextStage)();

            // Model parameters
            double mPreyInitialDensity        { 0.0 };
            double mPreyCompetitionFactor     { 0.3 };
            int mPreyReproductiveCapacity     { 10 };
            int mPreyReproductionRadius       { 2 };
            int mPredatorReproductiveCapacity { 10 };
            int mPredatorReproductionRadius   { 2 };
            int mFitnessRadius                { 3 };
            int NEIGHBORHOOD_SIZE             { (2 * mFitnessRadius + 1) * (2 * mFitnessRadius + 1) - 1 };

            // PSO parameters
            int mPredatorInitialSwarmSize       { 3 };
            int mPredatorMigrationTime          { 5 };
            int mPredatorMigrationCount         { 0 };
            float mPredatorInitialInertiaWeight { 0.9f };
            float mPredatorFinalInertiaWeight   { 0.2f };
            const float INERTIA_STEP            { (mPredatorInitialInertiaWeight - mPredatorFinalInertiaWeight) /
                mPredatorMigrationTime };
    };
}
