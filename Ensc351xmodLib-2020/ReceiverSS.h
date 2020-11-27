////////////////////////////////////////////////
// Generated by SmartState C++ Code Generator //
//                 DO NOT EDIT				  //
////////////////////////////////////////////////

#ifndef Receiver_SS_H
#define Receiver_SS_H

#include <ss_api.hxx>

/*Context*/
class ReceiverX;

namespace Receiver_SS
{
	using namespace smartstate;
	//State Mgr
	class ReceiverSS : public StateMgr
	{
		public:
			ReceiverSS(ReceiverX* ctx, bool startMachine=true);

			ReceiverX& getCtx() const;

		private:
			ReceiverX* myCtx;
	};

	//Base State
	class ReceiverBaseState : public BaseState
	{
		protected:
			ReceiverBaseState(){};
			ReceiverBaseState(const string& name, BaseState* parent, ReceiverSS* mgr);

		protected:
			ReceiverSS* getMgr(){return static_cast<ReceiverSS*>(myMgr);}
	};

	//States
	//------------------------------------------------------------------------
	class CompleteReceiverTopLevel_ReceiverSS : public virtual ReceiverBaseState
	{
			typedef ReceiverBaseState super;

		public:
			CompleteReceiverTopLevel_ReceiverSS(){};
			CompleteReceiverTopLevel_ReceiverSS(const string& name, BaseState* parent, ReceiverSS* mgr);

			virtual void onMessage(const Mesg& mesg);

			virtual void onEntry();
			virtual void onExit();

		//Transitions

		private:
			void onCONTMessage(const Mesg& mesg);
	};

	class KBcancelable_CompleteReceiverTopLevel : public virtual CompleteReceiverTopLevel_ReceiverSS
	{
			typedef CompleteReceiverTopLevel_ReceiverSS super;

		public:
			KBcancelable_CompleteReceiverTopLevel(){};
			KBcancelable_CompleteReceiverTopLevel(const string& name, BaseState* parent, ReceiverSS* mgr);

			virtual void onMessage(const Mesg& mesg);

			virtual void onEntry();
			virtual void onExit();

		//Transitions

		private:
			void onSERMessage(const Mesg& mesg);
			void onTMMessage(const Mesg& mesg);
	};

	class BETWEEN_KBcancelable : public virtual KBcancelable_CompleteReceiverTopLevel
	{
			typedef KBcancelable_CompleteReceiverTopLevel super;

		public:
			BETWEEN_KBcancelable(){};
			BETWEEN_KBcancelable(const string& name, BaseState* parent, ReceiverSS* mgr);

			virtual void onMessage(const Mesg& mesg);

			virtual void onEntry();
			virtual void onExit();

		//Transitions

		private:
			void onSERMessage(const Mesg& mesg);
	};

	class SERcancelable_BETWEEN : public virtual BETWEEN_KBcancelable
	{
			typedef BETWEEN_KBcancelable super;

		public:
			SERcancelable_BETWEEN(){};
			SERcancelable_BETWEEN(const string& name, BaseState* parent, ReceiverSS* mgr);

			virtual void onMessage(const Mesg& mesg);

			virtual void onEntry();
			virtual void onExit();

		//Transitions

		private:
			void onSERMessage(const Mesg& mesg);
	};

	class EOT_SERcancelable : public virtual SERcancelable_BETWEEN
	{
			typedef SERcancelable_BETWEEN super;

		public:
			EOT_SERcancelable(){};
			EOT_SERcancelable(const string& name, BaseState* parent, ReceiverSS* mgr);

			virtual void onMessage(const Mesg& mesg);

			virtual void onEntry();
			virtual void onExit();

		//Transitions

		private:
			void onSERMessage(const Mesg& mesg);
			void onKB_CMessage(const Mesg& mesg);
	};

	class FirstByte_SERcancelable : public virtual SERcancelable_BETWEEN
	{
			typedef SERcancelable_BETWEEN super;

		public:
			FirstByte_SERcancelable(){};
			FirstByte_SERcancelable(const string& name, BaseState* parent, ReceiverSS* mgr);

			virtual void onMessage(const Mesg& mesg);

			virtual void onEntry();
			virtual void onExit();

		//Transitions

		private:
			void onSERMessage(const Mesg& mesg);
			void onKB_CMessage(const Mesg& mesg);
	};

	class CAN_BETWEEN : public virtual BETWEEN_KBcancelable
	{
			typedef BETWEEN_KBcancelable super;

		public:
			CAN_BETWEEN(){};
			CAN_BETWEEN(const string& name, BaseState* parent, ReceiverSS* mgr);

			virtual void onMessage(const Mesg& mesg);

			virtual void onEntry();
			virtual void onExit();

		//Transitions

		private:
			void onTMMessage(const Mesg& mesg);
			void onKB_CMessage(const Mesg& mesg);
			void onSERMessage(const Mesg& mesg);
	};

	class WorkaroundTransient2_KBcancelable : public virtual KBcancelable_CompleteReceiverTopLevel
	{
			typedef KBcancelable_CompleteReceiverTopLevel super;

		public:
			WorkaroundTransient2_KBcancelable(){};
			WorkaroundTransient2_KBcancelable(const string& name, BaseState* parent, ReceiverSS* mgr);

			virtual void onMessage(const Mesg& mesg);

			virtual void onEntry();
			virtual void onExit();

		//Transitions

		private:
			void onCONTMessage(const Mesg& mesg);
	};

	class TimeoutTransient_CompleteReceiverTopLevel : public virtual CompleteReceiverTopLevel_ReceiverSS
	{
			typedef CompleteReceiverTopLevel_ReceiverSS super;

		public:
			TimeoutTransient_CompleteReceiverTopLevel(){};
			TimeoutTransient_CompleteReceiverTopLevel(const string& name, BaseState* parent, ReceiverSS* mgr);

			virtual void onMessage(const Mesg& mesg);

			virtual void onEntry();
			virtual void onExit();

		//Transitions

		private:
			void onSERMessage(const Mesg& mesg);
			void onTMMessage(const Mesg& mesg);
			void onKB_CMessage(const Mesg& mesg);
	};

	class WorkaroundTransient1_CompleteReceiverTopLevel : public virtual CompleteReceiverTopLevel_ReceiverSS
	{
			typedef CompleteReceiverTopLevel_ReceiverSS super;

		public:
			WorkaroundTransient1_CompleteReceiverTopLevel(){};
			WorkaroundTransient1_CompleteReceiverTopLevel(const string& name, BaseState* parent, ReceiverSS* mgr);

			virtual void onMessage(const Mesg& mesg);

			virtual void onEntry();
			virtual void onExit();

		//Transitions

		private:
			void onCONTMessage(const Mesg& mesg);
	};

};

#endif

//___________________________________vv^^vv___________________________________
