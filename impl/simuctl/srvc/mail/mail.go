package mail

import (
	"bytes"
	"fmt"
	"html/template"
	"log"
	"strings"

	"github.com/go-gomail/gomail"
)

type EmailParam struct {
	ServerHost string
	ServerPort int
	FromEmail  string
	FromPasswd string
	Toers      string
	CCers      string
}

var serverHost, fromEmail, fromPasswd string
var serverPort int

var m *gomail.Message

func InitEmail(ep *EmailParam) {
	toers := []string{}

	serverHost = ep.ServerHost
	serverPort = ep.ServerPort
	fromEmail = ep.FromEmail
	fromPasswd = ep.FromPasswd

	m = gomail.NewMessage()

	if len(ep.Toers) == 0 {
		return
	}

	for _, tmp := range strings.Split(ep.Toers, ",") {
		toers = append(toers, strings.TrimSpace(tmp))
	}
	//may be multi toer
	m.SetHeader("To", toers...)

	if len(ep.Toers) != 0 {

		for _, tmp := range strings.Split(ep.CCers, ",") {
			toers = append(toers, strings.TrimSpace(tmp))
		}
		//may be multi toer
		m.SetHeader("Cc", toers...)
	}

	//set sender
	m.SetAddressHeader("From", fromEmail, "")
}

func SendEmail(subject, body string) {

	m.SetHeader("Subject", subject)

	m.SetBody("text/html", body)

	d := gomail.NewPlainDialer(serverHost, serverPort, fromEmail, fromPasswd)
	err := d.DialAndSend(m)
	if err != nil {
		panic(err)
	}
}

func SendMultiLineMail(subject string, result []string) {
	data := new(bytes.Buffer)
	t := template.Must(template.New("").Parse(`<table>{{range.}}<tr><td>{{.}}</td></tr>{{end}}</table>`))
	if err := t.Execute(data, result); err != nil {
		log.Fatal(err)
	}
	fmt.Println(t, data)

	SendEmail(subject, string(data.Bytes()))
}

func InitMail() {
	serverHost := "smtp.126.com"
	serverPort := 25
	fromEmail := "jenkins_james@126.com"
	fromPasswd := "ZLPGRMBHFSLAYRFF"

	myToers := "samoren@126.com, jameszhangcn@126.com" //use , to split

	myCCers := "samoren@126.com"

	myEmail := &EmailParam{
		ServerHost: serverHost,
		ServerPort: serverPort,
		FromEmail:  fromEmail,
		FromPasswd: fromPasswd,
		Toers:      myToers,
		CCers:      myCCers,
	}
	InitEmail(myEmail)
}
