import React, { useState } from 'react';
import { Navbar, Nav, NavLink, NavItem, Button, Form, Input, Label, FormGroup } from 'reactstrap';


const api_url = 'http://131.181.190.87:3000';

function toggle(value) {
  if (value) {
    document.getElementById('toggleValue').innerHTML = "Already have an account?";
  }
  else {
    document.getElementById('toggleValue').innerHTML = "Don't have an account?";
  }
  document.getElementById('username').value = "";
  document.getElementById('password').value = "";

  return (value = !value);
}

function LoginOrRegister(props) {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");

  function Login(username, password) {
    const url = `${api_url}/user/login`;

    return fetch(url, {
      method: 'POST', headers: { accept: 'application/json', 'Content-type': 'application/json' },
      body: JSON.stringify({ email: username, password: password })
    })

    .then((res) => res.json())
    .then((res) => {
      if (res.error) {
        alert(res.message);
      }
      else {
        localStorage.setItem('token', res.token);
        window.location.href = '/logged_in';
      }
    });
  }

  function Register(username, password) {
    const url = `${api_url}/user/register`;

    return fetch(url, {
      method: 'POST',
      headers: {accept: 'application/json', 'Content-type': 'application/json' },
      body: JSON.stringify({ email: username, password: password })
    })

    .then((res) => res.json())
    .then((res) => {
      console.log(res);
      if (res.error) { } 
      else { }
    });
  }


  if (props.loginOrRegister) {
    return (
      <div className='Login-header'>
        <div>
          <Navbar expand='md'>
            <Nav className='Navbar' navbar>            
              <NavItem className='line'>
                <NavLink style={{ color:'white' }} href='/'>Home</NavLink>
              </NavItem>
              <NavItem className='line'>
                <NavLink style={{ color:'white' }} href='/quote'>Quote</NavLink>
              </NavItem>
              <NavItem>
                <NavLink style={{ color:'white' }} href='/price_history'>Price History (restricted)</NavLink>
              </NavItem>
              <NavItem className='navbar-space-right'>
                <NavLink className='line' style={{ color:'white'}} href='/login'>Login</NavLink>
              </NavItem>
              <NavItem>
                <NavLink style={{ color:'white' }} href='/login'>Register</NavLink>
              </NavItem>            
            </Nav>
          </Navbar>
        </div>
        <div>
          <br></br>
          <h3 style={{ color:'yellow' }}><center>Login</center></h3>
          <Form>
            <Label for='username'>Email</Label>
            <Input
              type='email'
              name='email'
              id='username'
              onChange={(e) => { setUsername(e.target.value); }} />
            <Label for='password'>Password</Label>
            <Input
              type='password'
              name='password'
              id='password'
              onChange={(e) => { setPassword(e.target.value); }} />
            <br></br>
            <Button
              className='container'
              type='submit'
              color='primary'
              onClick={(e) => {
                e.preventDefault();
                Login(username, password);
              }}>
              Submit
            </Button>
          </Form>
        </div>
      </div>
    );
  }

  else {
    return (
      <div className='Login-header'>
        <div>
          <Navbar expand='md'>
            <Nav className='Navbar' navbar>            
              <NavItem className='line'>
                <NavLink style={{ color:'white' }} href='/'>Home</NavLink>
              </NavItem>
              <NavItem className='line'>
                <NavLink style={{ color:'white' }} href='/quote'>Quote</NavLink>
              </NavItem>
              <NavItem>
                <NavLink style={{ color:'white' }} href='/price_history'>Price History (restricted)</NavLink>
              </NavItem>
              <NavItem className='navbar-space-right'>
                <NavLink className='line' style={{ color:'white'}} href='/login'>Login</NavLink>
              </NavItem>
              <NavItem>
                <NavLink style={{ color:'white' }} href='/login'>Register</NavLink>
              </NavItem>            
            </Nav>
          </Navbar>
        </div>
        <div>
          <br></br>
          <h3 style={{ color:'yellow' }}><center>Register</center></h3>
          <Form>
            <Label for='username'>Email</Label>
            <Input
              type='email'
              name='email'
              id='username'
              onChange={(e) => { setUsername(e.target.value); }} />
            <Label for='password'>Password</Label>
            <Input
              type='password'
              name='password'
              id='password'
              onChange={(e) => { setPassword(e.target.value); }} />
            <br></br>
            <Button
              className='container'
              type='submit'
              color='primary'
              onClick={(e) => {
                e.preventDefault();
                Register(username, password);
              }}>
              Submit
            </Button>
          </Form>
        </div>
      </div>      
    );
  }
}

export default function UserLoginOrRegister() {
  const [loginView, setLoginView] = useState(1);

  return (
    <div className='align-item-center'>
      <div className='mx-auto'>
        <Form>
          <LoginOrRegister loginOrRegister={loginView} />
          <FormGroup>
            <br></br>
            <center>
              <Button
                className='transparent'
                size='sm'
                onClick={() => { setLoginView(toggle(loginView)); }}>
                <p id='toggleValue' className='m-0'>
                  Don't have an account? 
                </p>
              </Button>
            </center>            
          </FormGroup>
        </Form>
      </div>
    </div>
  );
}