#pragma once

namespace snk {
    /// @brief Lazy resource
    /// @details
    ///     Sometimes you want a class to update its internal state
    ///     only after you set all parameters, the relevant parameters (like many Drawable classes).
    ///     C++ "mutable" keyword can be used for lazy resources, and
    ///     this class is a generic implementation of a lazy resource.
    /// @remarks On instantiation, the resource is marked as needing an update.
    class lazy_resource
    {
    protected:
        /// @remarks const to force to use only mutable fields. Better than allow to modify everything.
        virtual void update() const = 0;

        /// @brief When a parameter change, call this function for needing an update.
        void onChanged()
        {
            m_needUpdate = true;
        }

        /// @brief Call this function in top of functions that need that the instance is up-to-date.
        void ensureUpdated() const
        {
            if(m_needUpdate)
            {
                m_needUpdate = false;
                update();
            }
        }

    private:
        mutable bool m_needUpdate {true};
    };
}