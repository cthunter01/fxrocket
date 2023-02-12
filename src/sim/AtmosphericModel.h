#ifndef SIM_ATMOSPHERICMODEL_H
#define SIM_ATMOSPHERICMODEL_H

namespace sim
{

class AtmosphericModel
{
public:
   AtmosphericModel();
   virtual ~AtmosphericModel();

   virtual double getDensity(double altitude) = 0;
};

} // namespace sim
#endif // SIM_ATMOSPHERICMODEL_H