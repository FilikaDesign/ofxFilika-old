#pragma once

#include "ofMain.h"
#include "ofThread.h"
#include "ofxSMTP.h"


// Requires ofxSSL and ofxSMTP
// copy to data folder -> ssl folder from ofxSSL example
// copy to data folder -> example-smtp-account-settings.xml from ofxSMTP folder
// enable gmail security for third party apps
class ofxFilikaEmailSender : public ofThread
{
public:

	ofxSMTP::Client smtp;
	/// Create a ThreadedObject and initialize the member
	/// variable in an initialization list.

	string senderInfo;
	const char *C;
	string imgPath;
	string senderEmailAddress;


	ofEventListener smtpDeliveryListener;
	ofEventListener smtpExceptionListener;

	ofxFilikaEmailSender() : count(0), shouldThrowTestException(false)
	{

		// Register for SSL Context events.
		ofSSLManager::registerClientEvents(this);

		// Use the simple gmail settings (also works for any gmail based account).
		//ofx::SMTP::GmailSettings settings("filikatasarim@gmail.com", "02082011");
			// Load credentials and account settings from an xml or json file.
	 auto settings = ofxSMTP::Settings::loadFromXML("example-smtp-account-settings.xml");
		//auto settings = ofxSMTP::Settings::loadFromJSON("example-smtp-account-settings.json");

		// See SMTP::Settings for extensive configuration options.

		// Pass the settings to the client.
		smtp.setup(settings);

		// Register event callbacks for message delivery (or failure) events
		smtpDeliveryListener = smtp.events.onSMTPDelivery.newListener(this, &ofxFilikaEmailSender::onSMTPDelivery);
		smtpExceptionListener = smtp.events.onSMTPException.newListener(this, &ofxFilikaEmailSender::onSMTPException);
	}

	/// Start the thread.
	void start()
	{
		// Mutex blocking is set to true by default
		// It is rare that one would want to use startThread(false).
		startThread();
	}

	/// Signal the thread to stop.  After calling this method,
	/// isThreadRunning() will return false and the while loop will stop
	/// next time it has the chance to.
	void stop()
	{
		stopThread();
	}


	void onSMTPDelivery(std::shared_ptr<Poco::Net::MailMessage>& message)
	{
		ofLogNotice("ofApp::onSMTPDelivery") << "Message Sent: " << message->getSubject();
	}


	void onSMTPException(const ofxSMTP::ErrorArgs& evt)
	{
		ofLogError("ofApp::onSMTPException") << evt.error().displayText();

		if (evt.message())
		{
			ofLogError("ofApp::onSMTPException") << evt.message()->getSubject();
		}

	}


	void onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args)
	{
		ofLogNotice("ofApp::onClientVerificationError") << std::endl << ofToString(args);

		// If you want to proceed, you must allow the user to inspect the certificate
		// and set `args.setIgnoreError(true);` if they want to continue.

		// args.setIgnoreError(true);

	}

	void onSSLPrivateKeyPassphraseRequired(std::string& passphrase)
	{
		// If you want to proceed, you must allow the user to input the assign the private key's
		// passphrase to the `passphrase` argument.  For example:

		passphrase = ofSystemTextBoxDialog("Enter the Private Key Passphrase", "");

	}


	
	/// Our implementation of threadedFunction.
	void threadedFunction()
	{
		

		// You can construct complex messages using POCO's MailMessage object.
		// See http://pocoproject.org/docs/Poco.Net.MailMessage.html

		auto message = std::make_shared<Poco::Net::MailMessage>();

		// Encode the sender and set it.
		message->setSender(Poco::Net::MailMessage::encodeWord("filikatasarim@gmail.com",
			"UTF-8"));

		// Mark the primary recipient and add them.
		message->addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT,
			senderEmailAddress));

		// Encode the subject and set it.
		message->setSubject(("Ziraat Müzesi"));

		// Poco::Net::MailMessage will take ownership of the *PartSource files,
		// so you don't have to worry about deleting the pointers.
		message->addContent(new Poco::Net::StringPartSource("Merhaba Ziyaretçi"));

		// Poco::Net::MailMessage throws exceptions when a file is not found.
		// Thus, we need to add attachments in a try / catch block.

		try
		{
			message->addAttachment(Poco::Net::MailMessage::encodeWord(imgPath, "UTF-8"),
				new Poco::Net::FilePartSource(ofToDataPath(imgPath)));
		}
		catch (const Poco::OpenFileException& exc)
		{
			ofLogError("ofApp::keyPressed") << exc.name() << " : " << exc.displayText();
		}

		// Add an additional header, just because we can.
		message->add("X-Mailer", "Ziraat Bankası Müzesi");

		// Add the message to our outbox.
		smtp.send(message);
		
	}

	/// This drawing function cannot be called from the thread itself because
	/// it includes OpenGL calls (ofDrawBitmapString).
	void draw()
	{
		stringstream ss;

		ss << "I am a slowly increasing thread. " << endl;
		ss << "My current count is: ";

		if (lock())
		{
			// The mutex is now locked and the "count"
			// variable is protected.  Time to read it.
			ss << count;

			// Unlock the mutex.  This is only
			// called if lock() returned true above.
			unlock();
		}
		else
		{
			// If we reach this else statement, it means that we could not
			// lock our mutex, and so we do not need to call unlock().
			// Calling unlock without locking will lead to problems.
			ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
		}

		ofDrawBitmapString(ss.str(), 50, 56);
	}


	void sendMail(string mailTo, string fileName) {

		//senderInfo = "Herr Artut<" + mailTo + ">";
		//C = senderInfo.c_str(); // Sender Info

		senderEmailAddress = mailTo;
	
		imgPath = "D:\\Development\\of_v0.11.2_vs2017_release\\apps\\ziraat_Izmir_Projects\\ziraat_GreenScreen\\bin\\data\\" + fileName;// + dirName + "\\" + fileName;
		//imgPath = fileName;
		startThread();
		stopThread();


	}

	// Use unique_lock to protect a copy of count while getting a copy.
	int getCount()
	{
		unique_lock<std::mutex> lock(mutex);
		return count;
	}

	void throwTestException()
	{
		shouldThrowTestException = 1;
	}

protected:
	// A flag to check and see if we should throw a test exception.
	Poco::AtomicCounter shouldThrowTestException;

	// This is a simple variable that we aim to always access from both the
	// main thread AND this threaded object.  Therefore, we need to protect it
	// with the mutex.  In the case of simple numerical variables, some
	// garuntee thread safety for small integral types, but for the sake of
	// illustration, we use an int.  This int could represent ANY complex data
	// type that needs to be protected.
	//
	// Note, if we simply want to count in a thread-safe manner without worrying
	// about mutexes, we might use Poco::AtomicCounter instead.
	int count;

};
