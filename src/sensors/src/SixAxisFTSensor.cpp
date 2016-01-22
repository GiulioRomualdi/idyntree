/*
 * Copyright (C) 2015 Fondazione Istituto Italiano di Tecnologia
 * Author: Silvio Traversaro
 * email:  silvio.traversaro@iit.it
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
 */
#include "iDynTree/Core/Wrench.h"
#include "iDynTree/Sensors/SixAxisFTSensor.h"

#include "iDynTree/Core/Transform.h"

#include <iDynTree/Model/Traversal.h>
#include <iDynTree/Model/Link.h>


#include <cassert>


namespace iDynTree {

struct SixAxisForceTorqueSensor::SixAxisForceTorqueSensorPrivateAttributes
{
    // Name/id of the sensor
    std::string name;
    // Index of the two links at which the SixAxisForceTorqueSensor is connected
    int link1, link2, appliedWrenchLink;
    // Transform from the sensor
    Transform link1_H_sensor, link2_H_sensor;
    // Index of the parent junction
    int parent_junction_index;
    // Name of the parent junction
    std::string parent_junction_name;
    // Name of the two links at which the SixAxisForceTorqueSensor is connected
    std::string link1Name, link2Name;

};


SixAxisForceTorqueSensor::SixAxisForceTorqueSensor()
{
    this->pimpl = new SixAxisForceTorqueSensorPrivateAttributes;

    this->pimpl->name = "";
    this->pimpl->link1 = this->pimpl->link2 = this->pimpl->appliedWrenchLink = -1;
}

SixAxisForceTorqueSensor::SixAxisForceTorqueSensor(const SixAxisForceTorqueSensor& other):
    pimpl(new SixAxisForceTorqueSensorPrivateAttributes(*(other.pimpl)))
{

}

SixAxisForceTorqueSensor& SixAxisForceTorqueSensor::operator=(const SixAxisForceTorqueSensor& other)
{
    if(this != &other)
    {
        *pimpl = *(other.pimpl);
    }
    return *this;
}


SixAxisForceTorqueSensor::~SixAxisForceTorqueSensor()
{
    delete this->pimpl;
}

bool SixAxisForceTorqueSensor::setName(const std::string& _name)
{
    this->pimpl->name = _name;
    return true;
}


bool SixAxisForceTorqueSensor::setAppliedWrenchLink(const int applied_wrench_index)
{
    this->pimpl->appliedWrenchLink = applied_wrench_index;
    return true;
}


bool SixAxisForceTorqueSensor::setFirstLinkSensorTransform(const int link_index, const iDynTree::Transform& link_H_sensor) const
{
    this->pimpl->link1 = link_index;
    this->pimpl->link1_H_sensor = link_H_sensor;
    return true;
}

bool SixAxisForceTorqueSensor::setSecondLinkSensorTransform(const int link_index, const iDynTree::Transform& link_H_sensor) const
{
    this->pimpl->link2 = link_index;
    this->pimpl->link2_H_sensor = link_H_sensor;
    return true;
}

bool SixAxisForceTorqueSensor::setParentJoint(const std::string& parent)
{
    this->pimpl->parent_junction_name = parent;
    return true;
}

bool SixAxisForceTorqueSensor::setParentJointIndex(const int &parent_index)
{
    this->pimpl->parent_junction_index = parent_index;
    return true;
}

bool SixAxisForceTorqueSensor::isValid() const
{
    if( this->getName() == "" )
    {
        return false;
    }

    if( this->pimpl->link1 < 0 &&
        this->pimpl->link2 < 0 &&
        this->pimpl->appliedWrenchLink < 0 )
    {
        // Return false because the links is not appropriately setted
        return false;
    }

    if( this->pimpl->link1 !=
        this->pimpl->appliedWrenchLink &&
        this->pimpl->link2 != this->pimpl->appliedWrenchLink)
    {
        return false;
    }

    return true;
}

Sensor* SixAxisForceTorqueSensor::clone() const
{
    return (Sensor *)new SixAxisForceTorqueSensor(*this);
}


std::string SixAxisForceTorqueSensor::getName() const
{
    return this->pimpl->name;
}

SensorType SixAxisForceTorqueSensor::getSensorType() const
{
    return SIX_AXIS_FORCE_TORQUE;
}


int SixAxisForceTorqueSensor::getAppliedWrenchLink() const
{
    return this->pimpl->appliedWrenchLink;
}

std::string SixAxisForceTorqueSensor::getParentJoint() const
{
    return this->pimpl->parent_junction_name;
}

JointIndex SixAxisForceTorqueSensor::getParentJointIndex() const
{
    return this->pimpl->parent_junction_index;
}

bool SixAxisForceTorqueSensor::isLinkAttachedToSensor(const int link_index) const
{
    return (this->pimpl->link1 == link_index ||
            this->pimpl->link2 == link_index );
}


bool SixAxisForceTorqueSensor::getLinkSensorTransform(const int link_index, iDynTree::Transform& link_H_sensor) const
{
    if( this->pimpl->link1 == link_index )
    {
        link_H_sensor = this->pimpl->link1_H_sensor;
        return true;
    }

    if( this->pimpl->link2 == link_index )
    {
        link_H_sensor = this->pimpl->link2_H_sensor;
        return true;
    }

    return false;
}

bool SixAxisForceTorqueSensor::getWrenchAppliedOnLink(const int link_index,
                                                      const Wrench& measured_wrench,
                                                      iDynTree::Wrench& wrench_applied_on_link) const
{
    assert(this->isValid());

    Wrench buffered_wrench;

    if( link_index == this->pimpl->link1 )
    {
        buffered_wrench = this->pimpl->link1_H_sensor*measured_wrench;
        // If the measure wrench is the one applied on the other link, change sign
        if( this->getAppliedWrenchLink() != link_index )
        {
            wrench_applied_on_link = -buffered_wrench;
        }
        else
        {
            wrench_applied_on_link = buffered_wrench;
        }

        return true;
    }
    else if( link_index == this->pimpl->link2 )
    {
        buffered_wrench = this->pimpl->link2_H_sensor*measured_wrench;

        if( this->getAppliedWrenchLink() != link_index )
        {
            wrench_applied_on_link = -buffered_wrench;
        }
        else
        {
            wrench_applied_on_link = buffered_wrench;
        }

        return true;
    }
    else
    {
        wrench_applied_on_link = iDynTree::Wrench::Zero();
        return false;
    }
}

int SixAxisForceTorqueSensor::getFirstLinkIndex() const
{
    return this->pimpl->link1;
}

int SixAxisForceTorqueSensor::getSecondLinkIndex() const
{
    return this->pimpl->link2;
}

bool SixAxisForceTorqueSensor::setFirstLinkName(const std::string& name)
{
    this->pimpl->link1Name = name;
    return true;
}

bool SixAxisForceTorqueSensor::setSecondLinkName(const std::string& name)
{
    this->pimpl->link2Name = name;
    return true;
}

std::string SixAxisForceTorqueSensor::getFirstLinkName() const
{
    return this->pimpl->link1Name;
}

std::string SixAxisForceTorqueSensor::getSecondLinkName() const
{
    return this->pimpl->link2Name;
}

Wrench SixAxisForceTorqueSensor::predictMeasurement(const Traversal& traversal, const LinkInternalWrenches& intWrenches)
{
    Wrench simulated_measurement;

    //Check that the input size is consistent
    assert(this->isValid());
    assert(this->getFirstLinkIndex() >= 0 && this->getFirstLinkIndex() < traversal.getNrOfVisitedLinks());
    assert(this->getSecondLinkIndex() >= 0 && this->getSecondLinkIndex() < traversal.getNrOfVisitedLinks());


    // The intWrenches vector is assumed to be the output of the RNEADynamicPhase function called
    // with the passed traversal.
    // ie intWrench[i] is the force applied by link i on the link traversal.getParent(i),
    // expressed in the refernce frame of link i
    // From this information, we can "simulate" the output that we could expect on this sensor

    // First we get the two links attached to this ft sensor, and we check which one is the
    // parent and which one is the child in the dynamic_traversal Traversal
    LinkIndex child_link = LINK_INVALID_INDEX;
    LinkIndex parent_link = LINK_INVALID_INDEX;
    if( traversal.getParentLink(this->getFirstLinkIndex()) != 0 &&
        traversal.getParentLink(this->getFirstLinkIndex())->getIndex() == this->getSecondLinkIndex() )
    {
        child_link = this->getFirstLinkIndex();
        parent_link = this->getSecondLinkIndex();
    }
    else
    {
        assert(traversal.getParentLink(this->getSecondLinkIndex())->getIndex() == this->getFirstLinkIndex() );
        child_link = this->getSecondLinkIndex();
        parent_link = this->getFirstLinkIndex();
    }

    // if the child_link is the link to which the measured wrench is applied, the sign between the
    // measured_wrench and f[child_link] is consistent, otherwise we have to change the sign


    // To simulate the sensor, we have to translate f[child] in the sensor frame
    // with the appriopriate sign
    iDynTree::Transform child_link_H_sensor;
    this->getLinkSensorTransform(child_link,child_link_H_sensor);
    if( this->getAppliedWrenchLink() == parent_link  )
    {
        simulated_measurement = -(child_link_H_sensor.inverse()*intWrenches(child_link));
    }
    else
    {
        simulated_measurement = (child_link_H_sensor.inverse()*intWrenches(child_link));
        assert( this->getAppliedWrenchLink() == child_link );
    }

    return simulated_measurement;
}

std::string SixAxisForceTorqueSensor::toString(const Model& model) const
{
    std::stringstream ss;

    ss << "Sensor " << this->getName() << std::endl;
    ss << " is attached to joint " << this->getParentJoint() << " ( " << this->getParentJointIndex() << " ) " << std::endl;
    ss << " that connects  " << this->getFirstLinkName() << " ( " << this->getFirstLinkIndex() << " ) and "
                             << this->getSecondLinkName() << " ( " << this->getSecondLinkIndex() << std::endl;

    return ss.str();
}


}
