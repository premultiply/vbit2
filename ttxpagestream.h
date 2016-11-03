#ifndef _TTXPAGESTREAM_H_
#define _TTXPAGESTREAM_H_

#include <sys/stat.h>

#include "ttxpage.h"
#include "packet.h"

/** @brief Extends TTXPage to allow Service to iterate through this page.
 *  It adds iterators to the page and also timing control if it is a carousel.
 *  It also has features to help add, remove and update pages in a service.
 */
class TTXPageStream : public TTXPage
{
    public:
        /** Default constructor. Don't call this */
        TTXPageStream();
        /** Default destructor */
        virtual ~TTXPageStream();

        /** The normal constructor
         */
        TTXPageStream(std::string filename);

        /** Access _isCarousel
         * \return The current value of _isCarousel
         */
        bool GetCarouselFlag() { return _isCarousel; }

        /** Set _isCarousel
         * \param val New value to set
         */
        void SetCarouselFlag(bool val) { _isCarousel = val; }

        ///** Access _CurrentPage
         //* \return The current value of _CurrentPage
         //*/
        //TTXPageStream* GetCurrentPage(unsigned int line) { _CurrentPage->SetLineCounter(line);return _CurrentPage; }

        ///** Set _CurrentPage
         //* \param val New value to set
         //*/
        //void SetCurrentPage(TTXPageStream* val) { _CurrentPage = val; }


        /** Is the page a carousel?
         *  Don't confuse this with the _isCarousel flag which is used to mark when a page changes between single/carousel
         * \return True if there are subpages
         */
        inline bool IsCarousel(){return Getm_SubPage()!=NULL;};

        /** Set the time when this carousel expires
         *  ...which is the current time plus the cycle time
         */
        inline void SetTransitionTime(){_transitionTime=time(0)+m_cycletimeseconds;};

        /** Used to time carousels
         *  @return true if it is time to change carousel page
         */
        inline bool Expired(){return _transitionTime<=time(0);};

        /** Step to the next page in a carousel
				 *  Updates _CarouselPage;
         */
				void StepNextSubpage();

				/** This is used by mag */
				TTXPage* GetCarouselPage(){return _CarouselPage;};

				/** Get the array of 6 fastext links */
				int* GetLinkSet(){return m_fastextlinks;};

				/** Output a list of pages in this magazine
				*/
				void printList();

				/** Get the row from the page.
        * Carousels and main sequence pages are managed differently
				*/
				TTXLine* GetTxRow(uint8_t row);

        // The time that the file was modified.
				time_t GetModifiedTime(){return _modifiedTime;};
				void SetModifiedTime(time_t timeVal){_modifiedTime=timeVal;};

				bool LoadPage(std::string filename);

				void ClearExistsFlag(){_existsOnDrive=false;};
				void SetExistsFlag(){_existsOnDrive=true;};
				bool GetExistsFlag(){return _existsOnDrive;};

    protected:

    private:
			// Carousel control
			bool _isCarousel; //!< Member variable "_isCarousel" If
			// TTXPageStream* _CurrentPage; //!< Member variable "_currentPage" points to the subpage being transmitted

			time_t _transitionTime; // Records when the next carousel transition is due

			TTXPage* _CarouselPage; /// Pointer to the current subpage of a carousel
			
			// Things that affect the display list
			time_t _modifiedTime;   /// Poll this in case the source file changes (Used to detect updates)
			bool _existsOnDrive;	/// Used to mark if we found the file. (Used to detect deletions)
};

#endif // _TTXPAGESTREAM_H_
